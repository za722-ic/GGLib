#include "GG/UI/UI.h"
#include "GG/UI/Visitors/Visitor.h"

void GG::Visitor_GrowShrink_Vertical::visitForControl(Control* control)
{
	return;
}

void GG::Visitor_GrowShrink_Vertical::visitForLabel(Label* label)
{
	visitForControl(label);
}

// TODO: document that this grows the children of a container, not the container itself. this has the implication that the root can never be set to grow, which makes sense
void GG::Visitor_GrowShrink_Vertical::visitForFlexContainer(Container* container)
{
	resizeChildrenAlongCrossAxis(container);
	resizeChildrenAlongMainAxis(container);
}

void GG::Visitor_GrowShrink_Vertical::visitForAbsoluteContainer(Container* container)
{
	// none of the children of an absolute container are "resizable"
	// insofar that absolute containers do not resize their children. they'll go with whatever the user defined height is.
	// hence, there is nothing to do:
	return;

	// also, no need to ask the container's children to do anything.
	// they will be asked by the breadthFirstGrowShrink() function
}

void GG::Visitor_GrowShrink_Vertical::breadthFirstGrowShrink(Container* root)
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



void GG::Visitor_GrowShrink_Vertical::resizeChildrenAlongCrossAxis(Container* container)
{
	// this class is for resizing vertically. this function is for resizing along the cross axis. hence, the cross axis must be vertical. so the main axis must be horizontal.
	// if the main axis of the container is not horizotal, return
	if (container->layoutDirection != LayoutDirection::LEFT_TO_RIGHT) return;

	// find list of vertically resizable elements
	std::vector<Element*> verticallyResizableChildren;
	for (auto child : container->children)
	{
		if (child->isResizable(Axis::VERTICAL))
		{
			verticallyResizableChildren.push_back(child);
		}
	}

	// there are no resizable children, so nothing to do --> return
	if (verticallyResizableChildren.empty()) return;

	// find remaining height
	int remainingHeight = container->h - container->paddingTop - container->paddingBottom;

	// add remaining size to each child
	for (auto resizableChild : verticallyResizableChildren)
	{
		resizableChild->h += remainingHeight - resizableChild->h;
		resizableChild->h = (int)MoreMath::clamp(resizableChild->h, resizableChild->getMinHeight(), resizableChild->getMaxHeight());
	}
}

// TODO: this desperately needs refactoring
void GG::Visitor_GrowShrink_Vertical::resizeChildrenAlongMainAxis(Container* container)
{
	// this class is for resizing vertically. this function is for resizing along the main axis. hence, the main axis must be vertical 
	// if the main axis of the container is not vertical, return
	if (container->layoutDirection != LayoutDirection::TOP_TO_BOTTOM) return;

	// find remaining width
	int remainingHeight = container->h;
	remainingHeight -= (container->paddingTop + container->paddingBottom);
	remainingHeight -= (container->children.size() - 1) * container->gap;
	for (auto child : container->children) remainingHeight -= child->h;

	// find list of vertically resizable elements
	// TODO: if we are e.g., shrinking, then instead of adding all "resizable" elements, why not only add "shrinkable" elements (so something like if remaining < 0 then check child->isShrinkable else if remaining > 0 check if child->isGrowable)
	std::vector<Element*> verticallyResizableChildren;
	for (auto child : container->children)
	{
		if (child->isResizable(Axis::VERTICAL))
		{
			verticallyResizableChildren.push_back(child);
		}
	}

	// keep looping until either
	// 1. no remaining space to to shrink/grow children by
	// 2. there are no remaining children that can be resized (e.g.,  because they all grew to their max or min size)
	while (remainingHeight != 0 && verticallyResizableChildren.size() > 0)
	{
		bool shrinking = remainingHeight < 0; // else growing

		// find the smallest (if growing, because remaining space is positive) or largest (if shrinking, because remaining space is negative)
		int most;
		most = verticallyResizableChildren.at(0)->h;
		for (auto resizableChild : verticallyResizableChildren)
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

		// create a list of those elements with "most" size (where most may be most small or most large)
		std::vector<Element*> childrenThatAreMost;
		for (auto resizableChild : verticallyResizableChildren)
		{
			if (resizableChild->h == most) childrenThatAreMost.push_back(resizableChild);
		}

		// calculate total distribute to distribute amongst children. this may be negative if shrinking
		int heightToAdd;
		if (verticallyResizableChildren.size() > childrenThatAreMost.size())
		{
			// find the second smallest/larget
			int secondMost = shrinking ? -INT_MAX : INT_MAX;
			for (auto child : verticallyResizableChildren)
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

			heightToAdd = (secondMost - most) * childrenThatAreMost.size();
		}
		else // all elements of childrenThatAreMost are of equal size (perhaps because there is only a single element in resizableChildren)
		{
			heightToAdd = remainingHeight;
		}

		if (remainingHeight > 0)
		{
			heightToAdd = (int)MoreMath::clamp(heightToAdd, 0, remainingHeight);
		}
		else
		{
			heightToAdd = (int)MoreMath::clamp(heightToAdd, remainingHeight, 0);
		}
		remainingHeight -= heightToAdd;

		int sizeToAddPerElement = heightToAdd / std::ssize(childrenThatAreMost);

		for (auto childThatIsMost : childrenThatAreMost)
		{
			childThatIsMost->h += sizeToAddPerElement;

			// remove from resizables if this element has been grown passed max height, or shrunk past min height
			if (shrinking && childThatIsMost->h <= childThatIsMost->getMinHeight())
			{
				remainingHeight += childThatIsMost->h - childThatIsMost->getMinHeight();
				childThatIsMost->h = childThatIsMost->getMinHeight();
				std::erase(verticallyResizableChildren, childThatIsMost);
			}
			else if (!shrinking && childThatIsMost->h >= childThatIsMost->getMaxHeight())
			{
				remainingHeight += childThatIsMost->h - childThatIsMost->getMaxHeight();
				childThatIsMost->h = childThatIsMost->getMaxHeight();
				std::erase(verticallyResizableChildren, childThatIsMost);
			}
		}
	}
}
