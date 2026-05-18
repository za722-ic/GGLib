#include "ui/Visitor.h"
#include "ui/Container.h"
#include "ui/Control.h"

void Visitor_CalculateMinimumSizing::visitForControl(Control* control)
{
	return;
	if (control->isAutosize(Axis::HORIZONTAL))
	{
		// set wrap width based on max width
		// note: max width is INT_MAX by default! so if the user hasn't set a max size for the label, there is effectively no wrapping. SDL_TTF happens to interpret a wrapWidth of 0 as no wrapping
		int wrapWidth = std::max(control->getMinWidth(), control->getMaxWidth()); // if min > max, then use min instead (min always trumps max in UI)
		if (wrapWidth == INT_MAX) wrapWidth = 0;
		else wrapWidth -= (control->paddingLeft + control->paddingRight);
		control->tttext->setWrapWidth(wrapWidth);

		// calculate text width now that we have set wrap width
		int textWidth = control->tttext->getDimensions().first;

		// label width is can now be set to fit to text width, as per the rule of autosizing
		//control->minWidthCalculated = (textWidth + control->paddingLeft + control->paddingRight); // TODO: see equivalent function is horizontal autosize
		control->setPreferredWidth(textWidth + control->paddingLeft + control->paddingRight); // TODO: see equivalent function is horizontal autosize
	}

	if (control->isAutosize(Axis::VERTICAL))
	{
		// calculate text width now that we have set wrap width
		int textHeight= control->tttext->getDimensions().second;

		// label width is can now be set to fit to text width, as per the rule of autosizing
		//control->minHeightCalculated = (textHeight + control->paddingTop + control->paddingBottom); // TODO: see equivalent function is horizontal autosize
		control->setPreferredHeight(textHeight + control->paddingTop + control->paddingBottom); // TODO: see equivalent function is horizontal autosize
	}
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
