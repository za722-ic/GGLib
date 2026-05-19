#include "UI.h"
#include "ui/Visitor.h"

void Visitor_CalculateMinimumSizing::visitForControl(Control* control)
{
}

void Visitor_CalculateMinimumSizing::visitForLabel(Label* label)
{
	visitForControl(label);
}

void Visitor_CalculateMinimumSizing::visitForFlexContainer(Container* container)
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

void Visitor_CalculateMinimumSizing::visitForAbsoluteContainer(Container* container)
{
	// calculate size of children first (depth first order)
	for (auto child : container->children) child->accept(*this);

	container->minWidthCalculated = 0;
	container->minHeightCalculated = 0;
}
