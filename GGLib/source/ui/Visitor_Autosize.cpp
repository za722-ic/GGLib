#include "Visitor.h"
#include "Container.h"
#include "Control.h"
#include "Label.h"

void Visitor_Autosize::visitForLabel(Label* label)
{
	auto textDimensions = label->getTextDimensions();
	if (label->getAutoSizeHorizontal())
	{
		label->w = textDimensions.first;
	}
	if (label->getAutoSizeHorizontal())
	{
		label->h = textDimensions.second;
	}
}

void Visitor_Autosize::visitForControl(Control* control)
{
	return;
}

// if the container is set to autosize, then resize it accordingly
void Visitor_Autosize::visitForFlexContainer(Container* container)
{
	// first, perform autosizing of container's children (depth first order)
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

void Visitor_Autosize::visitForAbsoluteContainer(Container* container)
{
	// first, perform autosizing of container's children (depth first order)
	for (auto child : container->children) child->accept(*this);

	// Approach:
	// we find the rightmost edge of all immediate children, and then set the container to encompass that
	// we do not squish the container from the left, as that would violate the xAbs the user has set 
	// (e.g, if xAbs==50 for some child, it must stay the same after we resize the container. since locations are measured 
	// from the top-left, we cannot move the left or top of the container. so we only look at the container's bottom/right. 
	// hence, we look for the max bottom and max right)

	// autosize container horizontally
	int maxRight = 0; // init to 0, not container->w, since we want to be able to shrink the container in case e.g., an element got removed
	if (container->isAutosize(Axis::HORIZONTAL))
	{
		for (auto child : container->children)
		{
			maxRight = std::max(maxRight, child->xAbs + child->w);
		}
	}
	container->w = maxRight;

	// autosize container vertically
	int maxBottom = 0;
	if (container->isAutosize(Axis::VERTICAL))
	{
		for (auto child : container->children)
		{
			maxBottom = std::max(maxBottom, child->yAbs + child->h);
		}
	}
	container->h = maxBottom;

	// clamp container size to between min/max
	container->w = MoreMath::clamp(container->w, container->getMinWidth(), container->getMaxWidth());
	container->h = MoreMath::clamp(container->h, container->getMinHeight(), container->getMaxHeight());
}
