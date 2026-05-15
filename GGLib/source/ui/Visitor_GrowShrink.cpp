#include "Visitor.h"
#include "Container.h"
#include "Control.h"
#include "Label.h"

void Visitor_GrowShrink::visitForLabel(Label* label)
{
	visitForControl(label);
}

void Visitor_GrowShrink::visitForControl(Control* control)
{
	return;
}

// TODO: document that this grows the children of a container, not the container itself. this has the implication that the root can never be set to grow, which makes sense
void Visitor_GrowShrink::visitForFlexContainer(Container* container)
{
	resizeChildrenAlongCrossAxis(container);
	resizeChildrenAlongMainAxis(container);
}

void Visitor_GrowShrink::visitForAbsoluteContainer(Container* container)
{
	// none of the children of an absolute container are "resizable"
	// insofar that absolute containers do not resize their children. they'll go with whatever the user defined width is.
	// hence, there is nothing to do:
	return;

	// also, no need to ask the container's children to do anything.
	// they will be asked by the breadthFirstGrowShrink() function
}

void Visitor_GrowShrink::breadthFirstGrowShrink(Container* root)
{
	std::queue<Element*> q;
	q.push(root);

	while (!q.empty())
	{
		// perform the growing/shrinking of the Element
		q.front()->accept(*this);

		// add the Element's children to the list
		Visitor_GetChildren childGetter;
		q.front()->accept(childGetter);
		for (auto child : childGetter.children)
			q.push(child);

		q.pop();
	}
}

void Visitor_GrowShrink::resizeChildrenAlongCrossAxis(Container* container)
{
	// find list of resizable elements
	std::vector<Element*> resizableChildren;
	for (auto child : container->children)
	{
		if (container->layoutDirection == LayoutDirection::LEFT_TO_RIGHT)
		{
			if (child->isResizable(Axis::VERTICAL))
			{
				resizableChildren.push_back(child);
			}
		}
		else if (container->layoutDirection == LayoutDirection::TOP_TO_BOTTOM)
		{
			if (child->isResizable(Axis::HORIZONTAL))
			{
				resizableChildren.push_back(child);
			}
		}
	}

	// there are no resizable children, so nothing to do --> return
	if (resizableChildren.size() == 0) return;

	// find remaining size
	int remaining;
	if (container->layoutDirection == LayoutDirection::LEFT_TO_RIGHT)
	{
		remaining = container->h;
		remaining -= container->paddingTop;
		remaining -= container->paddingBottom;
	}
	else if (container->layoutDirection == LayoutDirection::TOP_TO_BOTTOM)
	{
		remaining = container->w;
		remaining -= container->paddingLeft;
		remaining -= container->paddingRight;
	}

	// add remaining size to each child
	if (container->layoutDirection == LayoutDirection::LEFT_TO_RIGHT)
	{
		for (auto resizableChild : resizableChildren)
		{
			resizableChild->h += remaining - resizableChild->h;
			resizableChild->h = (int)MoreMath::clamp(resizableChild->h, resizableChild->getMinHeight(), resizableChild->getMaxHeight());
		}
	}
	else if (container->layoutDirection == LayoutDirection::TOP_TO_BOTTOM)
	{
		for (auto resizableChild : resizableChildren)
		{
			resizableChild->w += remaining - resizableChild->w;
			resizableChild->w = (int)MoreMath::clamp(resizableChild->w, resizableChild->getMinWidth(), resizableChild->getMaxWidth());
		}
	}
}

// TODO: this desperately needs refactoring
void Visitor_GrowShrink::resizeChildrenAlongMainAxis(Container* container)
{
	// find remaining size 
	int remaining = 0;
	if (container->layoutDirection == LayoutDirection::LEFT_TO_RIGHT)
	{
		remaining = container->w;
		remaining -= container->paddingLeft;
		remaining -= container->paddingRight;
		for (auto child : container->children) remaining -= child->w;
		remaining -= (container->children.size() - 1) * container->gap;
	}
	else if (container->layoutDirection == LayoutDirection::TOP_TO_BOTTOM)
	{
		remaining = container->h;
		remaining -= container->paddingTop;
		remaining -= container->paddingBottom;
		for (auto child : container->children) remaining -= child->h;
		remaining -= (container->children.size() - 1) * container->gap;
	}

	// find list of resizable elements
	// TODO: if we are e.g., shrinking, then instead of adding all "resizable" elements, why not only add "shrinkable" elements (so something like if remaining < 0 then check child->isShrinkable else if remaining > 0 check if child->isGrowable)
	std::vector<Element*> resizableChildren;
	for (auto child : container->children)
	{
		if (container->layoutDirection == LayoutDirection::LEFT_TO_RIGHT)
		{
			if (child->isResizable(Axis::HORIZONTAL))
			{
				resizableChildren.push_back(child);
			}
		}
		else if (container->layoutDirection == LayoutDirection::TOP_TO_BOTTOM)
		{
			if (child->isResizable(Axis::VERTICAL))
			{
				resizableChildren.push_back(child);
			}
		}
	}

	// keep looping until either
	// 1. no remaining space to to shrink/grow children by
	// 2. there are no remaining children that can be resized (e.g.,  because they all grew to their max or min size)
	while (remaining != 0 && resizableChildren.size() > 0)
	{
		bool shrinking = remaining < 0; // else growing

		// find the smallest (if growing, because remaining space is positive) or largest (if shrinking, because remaining space is negative)
		int most;
		if (container->layoutDirection == LayoutDirection::LEFT_TO_RIGHT)
		{
			most = resizableChildren.at(0)->w;
			for (auto resizableChild : resizableChildren)
			{
				if (!shrinking) // remaining is positive
				{
					if (resizableChild->w < most) most = resizableChild->w; // most small
				}
				else // remaining is negative
				{
					if (resizableChild->w > most) most = resizableChild->w; // most large
				}
			}
		}
		else if (container->layoutDirection == LayoutDirection::TOP_TO_BOTTOM)
		{
			most = resizableChildren.at(0)->h;
			for (auto resizableChild : resizableChildren)
			{
				if (!shrinking) // remaining is positive
				{
					if (resizableChild->h < most) most = resizableChild->h; // most small
				}
				else // remaining is negative
				{
					if (resizableChild->h > most) most = resizableChild->h; // most large
				}
			}
		}

		// create a list of those elements with "most" size (where most may be most small or most large)
		std::vector<Element*> childrenThatAreMost;
		for (auto resizableChild : resizableChildren)
		{
			if (container->layoutDirection == LayoutDirection::LEFT_TO_RIGHT)
			{
				if (resizableChild->w == most) childrenThatAreMost.push_back(resizableChild);
			}
			else if (container->layoutDirection == LayoutDirection::TOP_TO_BOTTOM)
			{
				if (resizableChild->h == most) childrenThatAreMost.push_back(resizableChild);
			}
		}

		// calculate total distribute to distribute amongst children. this may be negative if shrinking
		int sizeToAdd;
		if (resizableChildren.size() > childrenThatAreMost.size())
		{
			// find the second smallest/larget
			int secondMost = shrinking ? -INT_MAX : INT_MAX;
			for (auto child : resizableChildren)
			{
				if (container->layoutDirection == LayoutDirection::LEFT_TO_RIGHT)
				{
					if (child->w == most) continue;
					if (!shrinking)
					{
						if (child->w < secondMost) secondMost = child->w;
					}
					else
					{
						if (child->w > secondMost) secondMost = child->w;
					}
				}
				else if (container->layoutDirection == LayoutDirection::TOP_TO_BOTTOM)
				{
					if (child->h == most) continue;
					if (!shrinking)
					{
						if (child->h < secondMost) secondMost = child->h;
					}
					else
					{
						if (child->h > secondMost) secondMost = child->h;
					}
				}

			}

			sizeToAdd = (secondMost - most) * childrenThatAreMost.size();
		}
		else // all elements of childrenThatAreMost are of equal size (perhaps because there is only a single element in resizableChildren)
		{
			sizeToAdd = remaining;
		}

		if (remaining > 0)
		{
			sizeToAdd = (int)MoreMath::clamp(sizeToAdd, 0, remaining);
		}
		else
		{
			sizeToAdd = (int)MoreMath::clamp(sizeToAdd, remaining, 0);
		}
		remaining -= sizeToAdd;

		int sizeToAddPerElement = sizeToAdd / childrenThatAreMost.size();

		for (auto childThatIsMost : childrenThatAreMost)
		{
			if (container->layoutDirection == LayoutDirection::LEFT_TO_RIGHT)
			{
				childThatIsMost->w += sizeToAddPerElement;

				// remove from resizables if this element has been grown passed max width, or shrunk past min width
				if (shrinking && childThatIsMost->w <= childThatIsMost->getMinWidth())
				{
					remaining += childThatIsMost->w - childThatIsMost->getMinWidth();
					childThatIsMost->w = childThatIsMost->getMinWidth();
					std::erase(resizableChildren, childThatIsMost);
				}
				else if (!shrinking && childThatIsMost->w >= childThatIsMost->getMaxWidth())
				{
					remaining += childThatIsMost->w - childThatIsMost->getMaxWidth();
					childThatIsMost->w = childThatIsMost->getMaxWidth();
					std::erase(resizableChildren, childThatIsMost);
				}
			}
			else if (container->layoutDirection == LayoutDirection::TOP_TO_BOTTOM)
			{
				childThatIsMost->h += sizeToAddPerElement;

				// remove from resizables if this element has been grown passed max height, or shrunk past min height
				if (shrinking && childThatIsMost->h <= childThatIsMost->getMinHeight())
				{
					remaining += childThatIsMost->h - childThatIsMost->getMinHeight();
					childThatIsMost->h = childThatIsMost->getMinHeight();
					std::erase(resizableChildren, childThatIsMost);
				}
				else if (!shrinking && childThatIsMost->h >= childThatIsMost->getMaxHeight())
				{
					remaining += childThatIsMost->h - childThatIsMost->getMaxHeight();
					childThatIsMost->h = childThatIsMost->getMaxHeight();
					std::erase(resizableChildren, childThatIsMost);
				}
			}
		}
	}
}
