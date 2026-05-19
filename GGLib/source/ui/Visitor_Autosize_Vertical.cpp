#include "UI.h"
#include "ui/Visitor.h"

void Visitor_Autosize_Vertical::visitForControl(Control* control)
{
}

void Visitor_Autosize_Vertical::visitForLabel(Label* label)
{
	// if autosize enabled, then label height depends on text height
	if (label->isAutosize(Axis::VERTICAL))
	{
		// set label height based on text height
		int textHeight = label->tttext->getDimensions().second;
		label->h = textHeight + label->paddingBottom + label->paddingTop;
		
		// clamp label height
		label->h = MoreMath::clamp(label->h, label->getMinHeight(), label->getMaxHeight());
	}
}

// if the container is set to autosize, then resize it accordingly
void Visitor_Autosize_Vertical::visitForFlexContainer(Container* container)
{
	// first, perform autosizing of container's children (depth first order)
	for (auto child : container->children) child->accept(*this);

	// calculate total height of this container
	int totalHeight = 0;
	if (container->layoutDirection == LayoutDirection::TOP_TO_BOTTOM)
	{
		int totalVerticalPadding = container->paddingTop + container->paddingBottom;

		int totalVerticalGaps = (container->children.size() - 1) * container->gap;

		int totalHeightOfChildren = 0;
		for (auto child : container->children) totalHeightOfChildren += child->h;

		totalHeight = totalHeightOfChildren + totalVerticalGaps + totalVerticalPadding;
	}
	else if (container->layoutDirection == LayoutDirection::LEFT_TO_RIGHT)
	{
		int totalVerticalPadding = container->paddingTop + container->paddingBottom;

		int maxHeightOfChildren = 0;
		for (auto child : container->children) maxHeightOfChildren = std::max(maxHeightOfChildren, child->h);

		totalHeight = maxHeightOfChildren + totalVerticalPadding;
	}

	// only adjust the size if container is autosize 
	// TODO: above calculations are redundant if verticalAutoSize is false. this is easy to solve, but the code could be more elegant if the below clamping operation can be skipped... so can it?
	if (container->verticalAutosize) container->h = totalHeight;

	// clamp to between min/max width
	container->h = MoreMath::clamp(container->h, container->getMinHeight(), container->getMaxHeight());
}

void Visitor_Autosize_Vertical::visitForAbsoluteContainer(Container* container)
{
	// first, perform autosizing of container's children (depth first order)
	for (auto child : container->children) child->accept(*this);

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
	container->h = MoreMath::clamp(container->h, container->getMinHeight(), container->getMaxHeight());
}
