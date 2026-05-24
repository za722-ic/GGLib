#include "GG/UI/UI.h"
#include "GG/UI/Visitor.h"

void Visitor_GrowShrink_Horizontal::visitForControl(Control* control)
{
}

void Visitor_GrowShrink_Horizontal::visitForLabel(Label* label)
{
	visitForControl(label);
}

// TODO: document that this grows the children of a container, not the container itself. this has the implication that the root can never be set to grow, which makes sense
void Visitor_GrowShrink_Horizontal::visitForFlexContainer(Container* container)
{
	resizeChildrenAlongCrossAxis(container);
	resizeChildrenAlongMainAxis(container);
}

void Visitor_GrowShrink_Horizontal::visitForAbsoluteContainer(Container* container)
{
	// none of the children of an absolute container are "resizable"
	// insofar that absolute containers do not resize their children. they'll go with whatever the user defined width is.
	// hence, there is nothing to do:
	return;

	// also, no need to ask the container's children to do anything.
	// they will be asked by the breadthFirstGrowShrink() function
}

void Visitor_GrowShrink_Horizontal::breadthFirstGrowShrink(Container* root)
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



void Visitor_GrowShrink_Horizontal::resizeChildrenAlongCrossAxis(Container* container)
{
	// this class is for resizing horizontally. this function is for resizing along the cross axis. hence, the cross axis must be horizontal. so the main axis must be vertical.
	// if the main axis of the container is not vertical, return
	if (container->layoutDirection != LayoutDirection::TOP_TO_BOTTOM) return;

	// find list of horizontally resizable elements
	std::vector<Element*> horizontallyResizableChildren;
	for (auto child : container->children)
	{
		if (child->isResizable(Axis::HORIZONTAL))
		{
			horizontallyResizableChildren.push_back(child);
		}
	}

	// there are no resizable children, so nothing to do --> return
	if (horizontallyResizableChildren.empty()) return;

	// find remaining width
	int remainingWidth = container->w - container->paddingLeft - container->paddingRight;

	// add remaining size to each child
	for (auto resizableChild : horizontallyResizableChildren)
	{
		resizableChild->w += remainingWidth - resizableChild->w;
		resizableChild->w = (int)MoreMath::clamp(resizableChild->w, resizableChild->getMinWidth(), resizableChild->getMaxWidth());
	}
}

// TODO: this desperately needs refactoring
void Visitor_GrowShrink_Horizontal::resizeChildrenAlongMainAxis(Container* container)
{
	// this class is for resizing horizontally. this function is for resizing along the main axis. hence, the main axis must be horizontal
	// if the main axis of the container is not horizontal , return
	if (container->layoutDirection != LayoutDirection::LEFT_TO_RIGHT) return;

	// find remaining width
	int remainingWidth = container->w;
	remainingWidth -= (container->paddingLeft + container->paddingRight);
	remainingWidth -= (container->children.size() - 1) * container->gap;
	for (auto child : container->children)
		remainingWidth -= child->w;

	// find list of horizontally resizable elements
	// TODO: if we are e.g., shrinking, then instead of adding all "resizable" elements, why not only add "shrinkable" elements (so something like if remaining < 0 then check child->isShrinkable else if remaining > 0 check if child->isGrowable)
	std::vector<Element*> horizontallyResizableChildren;
	for (auto child : container->children)
	{
		if (child->isResizable(Axis::HORIZONTAL))
		{
			horizontallyResizableChildren.push_back(child);
		}
	}

	// keep looping until either
	// 1. no remaining space to to shrink/grow children by
	// 2. there are no remaining children that can be resized (e.g.,  because they all grew to their max or min size)
	while (remainingWidth != 0 && horizontallyResizableChildren.size() > 0)
	{
		bool shrinking = remainingWidth < 0; // else growing

		// find the smallest (if growing, because remaining space is positive) or largest (if shrinking, because remaining space is negative)
		int most;
		most = horizontallyResizableChildren.at(0)->w;
		for (auto resizableChild : horizontallyResizableChildren)
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

		// create a list of those elements with "most" size (where most may be most small or most large)
		std::vector<Element*> childrenThatAreMost;
		for (auto resizableChild : horizontallyResizableChildren)
		{
			if (resizableChild->w == most) childrenThatAreMost.push_back(resizableChild);
		}

		// calculate total distribute to distribute amongst children. this may be negative if shrinking
		int widthToAdd;
		if (horizontallyResizableChildren.size() > childrenThatAreMost.size())
		{
			// find the second smallest/larget
			int secondMost = shrinking ? -INT_MAX : INT_MAX;
			for (auto child : horizontallyResizableChildren)
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

			widthToAdd = (secondMost - most) * childrenThatAreMost.size();
		}
		else // all elements of childrenThatAreMost are of equal size (perhaps because there is only a single element in resizableChildren)
		{
			widthToAdd = remainingWidth;
		}

		if (remainingWidth > 0)
		{
			widthToAdd = (int)MoreMath::clamp(widthToAdd, 0, remainingWidth);
		}
		else
		{
			widthToAdd = (int)MoreMath::clamp(widthToAdd, remainingWidth, 0);
		}
		remainingWidth -= widthToAdd;

		int sizeToAddPerElement = widthToAdd / std::ssize(childrenThatAreMost);

		for (auto childThatIsMost : childrenThatAreMost)
		{
			childThatIsMost->w += sizeToAddPerElement;

			// remove from resizables if this element has been grown passed max width, or shrunk past min width
			if (shrinking && childThatIsMost->w <= childThatIsMost->getMinWidth())
			{
				remainingWidth += childThatIsMost->w - childThatIsMost->getMinWidth();
				childThatIsMost->w = childThatIsMost->getMinWidth();
				std::erase(horizontallyResizableChildren, childThatIsMost);
			}
			else if (!shrinking && childThatIsMost->w >= childThatIsMost->getMaxWidth())
			{
				remainingWidth += childThatIsMost->w - childThatIsMost->getMaxWidth();
				childThatIsMost->w = childThatIsMost->getMaxWidth();
				std::erase(horizontallyResizableChildren, childThatIsMost);
			}
		}
	}
}
