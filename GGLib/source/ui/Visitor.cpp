#include "Visitor.h"
#include "Container.h"
#include "Control.h"

void Visitor_GetChildren::visitForControl(Control *control)
{
	children.clear();
}

void Visitor_GetChildren::visitForContainer(Container *container)
{
	this->children = container->children;
}

///////////////////////////////////////////////////

// TODO: not all controls need to listen to all events.
// TODO: we need to remove controls when they are destroyed --> need another vistor for that
void Visitor_SetInputManager::visitForControl(Control* control)
{
	inputManager->addKeyEventListener(control);
	inputManager->addMouseEventListener(control);
	inputManager->addTextInputEventListener(control);
	inputManager->addScrollEventListener(control);
}

void Visitor_SetInputManager::visitForContainer(Container* container)
{
	return;
}

///////////////////////////////////////////////////



void Visitor_DestroySelfAndChildren::visitForControl(Control *control)
{
	delete control;
}

void Visitor_DestroySelfAndChildren::visitForContainer(Container *container)
{
	for (auto child : container->children) delete child;

	delete container;
}



///////////////////////////////////////////////////



void Visitor_CalculateMinimumSizing::visitForControl(Control* control)
{
	return;
}

void Visitor_CalculateMinimumSizing::visitForContainer(Container* container)
{
	// calculate size of children first (depth first order)
	for (auto child : container->children) child->accept(*this);

	// calculate size along layout axis of current element
	int totalMinimumSizeOfChildren = 0;
	int totalSizeOfPaddingAlongMainAxis = 0;
	int totalSizeOfGaps = (container->children.size() - 1) * container->gap;
	if (container->layoutDirection == LayoutDirection::LEFT_TO_RIGHT)
	{
		for (auto child : container->children) totalMinimumSizeOfChildren += child->getMinWidth();
		totalSizeOfPaddingAlongMainAxis = container->paddingLeft + container->paddingRight;
	}
	else if (container->layoutDirection == LayoutDirection::TOP_TO_BOTTOM)
	{
		for (auto child : container->children) totalMinimumSizeOfChildren += child->getMinHeight();
		totalSizeOfPaddingAlongMainAxis = container->paddingTop + container->paddingBottom;
	}
	int minimumSizeAlongMainAxis = totalMinimumSizeOfChildren + totalSizeOfGaps + totalSizeOfPaddingAlongMainAxis;

	// calculate across layout axis of current element
	int maxSizeOfChildren = 0;
	int totalSizeOfPaddingAlongCrossAxis = 0;
	if (container->layoutDirection == LayoutDirection::LEFT_TO_RIGHT)
	{
		for (auto child : container->children) maxSizeOfChildren = std::max(maxSizeOfChildren, child->getMinHeight());
		totalSizeOfPaddingAlongCrossAxis = container->paddingTop + container->paddingBottom;
	}
	else if (container->layoutDirection == LayoutDirection::TOP_TO_BOTTOM)
	{
		for (auto child : container->children) maxSizeOfChildren = std::max(maxSizeOfChildren, child->getMinWidth());
		totalSizeOfPaddingAlongCrossAxis = container->paddingLeft + container->paddingRight;
	}
	int minimumSizeAlongCrossAxis = maxSizeOfChildren + totalSizeOfPaddingAlongCrossAxis;

	if (container->layoutDirection == LayoutDirection::LEFT_TO_RIGHT)
	{
		container->minWidthCalculated = minimumSizeAlongMainAxis;
		container->minHeightCalculated = minimumSizeAlongCrossAxis;
	}
	else if (container->layoutDirection == LayoutDirection::TOP_TO_BOTTOM)
	{
		container->minWidthCalculated = minimumSizeAlongCrossAxis;
		container->minHeightCalculated = minimumSizeAlongMainAxis;
	}
}



///////////////////////////////////////////////////



void Visitor_Autosize::visitForControl(Control* control)
{
	return;
}

// if the container is set to autosize, then resize it accordingly
void Visitor_Autosize::visitForContainer(Container* container)
{
	// calculate size of children first (depth first order)
	for (auto child : container->children) child->accept(*this);

	// calculate size along layout axis of current element
	int totalSizeOfChildren = 0;
	int totalSizeOfPaddingAlongMainAxis = 0;
	int totalSizeOfGaps = (container->children.size() - 1) * container->gap;
	if (container->layoutDirection == LayoutDirection::LEFT_TO_RIGHT)
	{
		for (auto child : container->children) totalSizeOfChildren += child->w;
		totalSizeOfPaddingAlongMainAxis = container->paddingLeft + container->paddingRight;
	}
	else if (container->layoutDirection == LayoutDirection::TOP_TO_BOTTOM)
	{
		for (auto child : container->children) totalSizeOfChildren += child->h;
		totalSizeOfPaddingAlongMainAxis = container->paddingTop + container->paddingBottom;
	}
	int sizeAlongMainAxis = totalSizeOfChildren + totalSizeOfGaps + totalSizeOfPaddingAlongMainAxis;

	// calculate across layout axis of current element
	int maxSizeOfChildren = 0;
	int totalSizeOfPaddingAlongCrossAxis = 0;
	if (container->layoutDirection == LayoutDirection::LEFT_TO_RIGHT)
	{
		for (auto child : container->children) maxSizeOfChildren = std::max(maxSizeOfChildren, child->h);
		totalSizeOfPaddingAlongCrossAxis = container->paddingTop + container->paddingBottom;
	}
	else if (container->layoutDirection == LayoutDirection::TOP_TO_BOTTOM)
	{
		for (auto child : container->children) maxSizeOfChildren = std::max(maxSizeOfChildren, child->w);
		totalSizeOfPaddingAlongCrossAxis = container->paddingLeft + container->paddingRight;
	}
	int sizeAlongCrossAxis = maxSizeOfChildren + totalSizeOfPaddingAlongCrossAxis;

	// only adjust the size if container is autosize 
	// TODO: this is inefficient --> why do all the above calculations if sizing mode is fixed --> were just gonna discard the result anyway
	if (container->layoutDirection == LayoutDirection::LEFT_TO_RIGHT)
	{
		if (container->horizontalAutosize) container->w = sizeAlongMainAxis;
		if (container->verticalAutosize) container->h = sizeAlongCrossAxis;
	}
	else if (container->layoutDirection == LayoutDirection::TOP_TO_BOTTOM)
	{
		if (container->horizontalAutosize) container->w = sizeAlongCrossAxis;
		if (container->verticalAutosize) container->h = sizeAlongMainAxis;
	}

	container->w = MoreMath::clamp(container->w, container->getMinWidth(), container->getMaxWidth());
	container->h = MoreMath::clamp(container->h, container->getMinHeight(), container->getMaxHeight());
}




///////////////////////////////////////////////////



void Visitor_GrowShrink::visitForControl(Control* control)
{
	return;
}

// TODO: document that this grows the children of a container, not the container itself. this has the implication that the root can never be set to grow, which makes sense
void Visitor_GrowShrink::visitForContainer(Container* container)
{
	resizeChildrenAlongCrossAxis(container);
	resizeChildrenAlongMainAxis(container);
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

void Visitor_GrowShrink::resizeChildrenAlongMainAxis(Container* container)
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

void Visitor_GrowShrink::resizeChildrenAlongCrossAxis(Container* container)
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



///////////////////////////////////////////////////


void Visitor_Positions::visitForControl(Control* control)
{
	control->screenX = control->x + offsetX;
	control->screenY = control->y + offsetY;
}

void Visitor_Positions::visitForContainer(Container* container)
{
	container->screenX = container->x + offsetX;
	container->screenY = container->y + offsetY;

	// TODO refactor alignment calculations
	// calculate alignment offset ALONG the layout axis
	int inlineAlignmentOffsetX = 0;
	int inlineAlignmentOffsetY = 0;
	if (container->layoutDirection == LayoutDirection::LEFT_TO_RIGHT && container->horizontalAlignmentMode != HAlignmentMode::LEFT)
	{
		int remainingWidth = container->w;
		remainingWidth -= container->paddingLeft;
		remainingWidth -= container->paddingRight;
		for (auto child : container->children) remainingWidth -= child->w;
		remainingWidth -= (container->children.size() - 1) * container->gap;

		if (container->horizontalAlignmentMode == HAlignmentMode::CENTER)
		{
			inlineAlignmentOffsetX += remainingWidth / 2;
		}
		else if (container->horizontalAlignmentMode == HAlignmentMode::RIGHT)
		{
			inlineAlignmentOffsetX += remainingWidth;
		}
	}
	else if (container->layoutDirection == LayoutDirection::TOP_TO_BOTTOM && container->verticalAlignmentMode != VAlignmentMode::TOP)
	{
		int remainingHeight = container->h;
		remainingHeight -= container->paddingTop;
		remainingHeight -= container->paddingBottom;
		for (auto child : container->children) remainingHeight -= child->h;
		remainingHeight -= (container->children.size() - 1) * container->gap;

		if (container->verticalAlignmentMode == VAlignmentMode::CENTER)
		{
			inlineAlignmentOffsetY += remainingHeight / 2;
		}
		else if (container->verticalAlignmentMode == VAlignmentMode::BOTTOM)
		{
			inlineAlignmentOffsetY += remainingHeight;
		}
	}

	// render everything else
	offsetX = container->screenX + container->paddingLeft + inlineAlignmentOffsetX;
	offsetY = container->screenY + container->paddingTop + inlineAlignmentOffsetY;
	for (auto child : container->children)
	{
		// calculate alignment offset ACROSS the layout axis
		int acrossAlignmentOffsetX = 0;
		int acrossAlignmentOffsetY = 0;
		if (container->layoutDirection == LayoutDirection::LEFT_TO_RIGHT && container->verticalAlignmentMode != VAlignmentMode::TOP)
		{
			int remainingHeight = container->h;
			remainingHeight -= container->paddingTop;
			remainingHeight -= container->paddingBottom;
			remainingHeight -= child->h;

			if (container->verticalAlignmentMode == VAlignmentMode::CENTER)
			{
				acrossAlignmentOffsetY += remainingHeight / 2;
			}
			else if (container->verticalAlignmentMode == VAlignmentMode::BOTTOM)
			{
				acrossAlignmentOffsetY += remainingHeight;
			}
		}
		else if (container->layoutDirection == LayoutDirection::TOP_TO_BOTTOM && container->horizontalAlignmentMode != HAlignmentMode::LEFT)
		{
			int remainingWidth = container->w;
			remainingWidth -= container->paddingLeft;
			remainingWidth -= container->paddingRight;
			remainingWidth -= child->w;

			if (container->horizontalAlignmentMode == HAlignmentMode::CENTER)
			{
				acrossAlignmentOffsetX += remainingWidth / 2;
			}
			else if (container->horizontalAlignmentMode == HAlignmentMode::RIGHT)
			{
				acrossAlignmentOffsetX += remainingWidth;
			}
		}

		// TODO: maybe the intent here would be better represented by using a stack?
		int oldOffsetX = offsetX;
		int oldOffsetY = offsetY;
		offsetX += acrossAlignmentOffsetX;
		offsetY += acrossAlignmentOffsetY;
		//child->calculatePositions(offsetX + acrossAlignmentOffsetX, offsetY + acrossAlignmentOffsetY);
		child->accept(*this);
		offsetX = oldOffsetX;
		offsetY = oldOffsetY;

		if (container->layoutDirection == LayoutDirection::LEFT_TO_RIGHT)
			offsetX += child->w + container->gap;
		else if (container->layoutDirection == LayoutDirection::TOP_TO_BOTTOM)
			offsetY += child->h + container->gap;
	}
}

