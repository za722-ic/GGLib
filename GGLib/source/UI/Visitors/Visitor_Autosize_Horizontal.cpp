#include "GG/UI/UI.h"
#include "GG/UI/Visitors/Visitor.h"

void Visitor_Autosize_Horizontal::visitForControl(Control* control)
{
}

void Visitor_Autosize_Horizontal::visitForLabel(Label* label)
{
	// if autosize enabled, then control width depends on text width
	// (if autosize disabled, then text width (wrapWidth) depends on label width --> will be set in wrap pass)
	if (label->isAutosize(Axis::HORIZONTAL))
	{
		// set wrap width based on max width
		// since we're using min/max width here, there is no need to clamp the label width later TODO double check this is true
		// (ASSUMING horizontal autosize enabled --> if it is disabled, then it'll get clamped during growshink sizing)
		int wrapWidth = std::max(label->minWidth.value_or(0), label->maxWidth.value_or(INT_MAX)); // if min > max, then use min instead (min always trumps max in UI)
		if (wrapWidth == INT_MAX) wrapWidth = 0; // note: max width is INT_MAX by default! so if the user hasn't set a max size for the label, there is effectively no wrapping. SDL_TTF happens to interpret a wrapWidth of 0 as no wrapping
		else wrapWidth -= (label->paddingLeft + label->paddingRight); // TODO negative wrap width possible?
		label->tttext->setWrapWidth(wrapWidth);

		// calculate text width now that we have set wrap width
		int textWidth = label->tttext->getDimensions().first;

		// label width can now be set to fit to text width, as per the rule of autosizing
		int labelWidth = textWidth + label->paddingLeft + label->paddingRight;
		label->minWidthCalculated = labelWidth;
		label->maxWidthCalculated = labelWidth;
		label->w = labelWidth;
	}
}

// if the container is set to autosize, then resize it accordingly
void Visitor_Autosize_Horizontal::visitForFlexContainer(Container* container)
{
	// first, perform autosizing of container's children (depth first order)
	for (auto child : container->children) child->accept(*this);

	// calculate total width of this container
	int totalWidth = 0;
	if (container->layoutDirection == LayoutDirection::LEFT_TO_RIGHT)
	{
		int totalHorizontalPadding = container->paddingLeft + container->paddingRight;

		int totalHorizontalGaps = (container->children.size() - 1) * container->gap;

		int totalWidthOfChildren = 0;
		for (auto child : container->children) totalWidthOfChildren += child->w;

		totalWidth = totalWidthOfChildren + totalHorizontalGaps + totalHorizontalPadding;
	}
	else if (container->layoutDirection == LayoutDirection::TOP_TO_BOTTOM)
	{
		int totalHorizontalPadding = container->paddingLeft + container->paddingRight;

		int maxWidthOfChildren = 0;
		for (auto child : container->children) maxWidthOfChildren = std::max(maxWidthOfChildren, child->w);

		totalWidth = maxWidthOfChildren + totalHorizontalPadding;
	}

	// only adjust the size if container is autosize 
	// TODO: above calculations are redundant if horizontalAutosize is false. this is easy to solve, but the code could be more elegant if the below clamping operation can be skipped... so can it?
	if (container->horizontalAutosize) 
		container->w = totalWidth;

	// clamp to between min/max width
	container->w = MoreMath::clamp(container->w, container->getMinWidth(), container->getMaxWidth());
}

// Approach:
// we find the rightmost edge of all immediate children, and then set the container to encompass that
// we do not squish the container from the left, as that would violate the xAbs the user has set 
// (e.g, if xAbs==50 for some child, it must stay the same after we resize the container. since locations are measured 
// from the top-left, we cannot move the left or top of the container. so we only look at the container's bottom/right. 
// hence, we look for the max bottom and max right)
void Visitor_Autosize_Horizontal::visitForAbsoluteContainer(Container* container)
{
	// first, perform autosizing of container's children (depth first order)
	for (auto child : container->children) child->accept(*this);

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

	// clamp container size to between min/max
	container->w = MoreMath::clamp(container->w, container->getMinWidth(), container->getMaxWidth());
}
