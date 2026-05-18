#include "Visitor.h"
#include "Container.h"
#include "Control.h"

void Visitor_Autosize_Horizontal::visitForControl(Control* control)
{
	// if autosize enabled, then control width depends on text width
	//if (control->isAutosize(Axis::HORIZONTAL))
	//{
	//	control->tttext->setWrapWidth(0);
	//	int textWidth = control->tttext->getDimensions().first;

	//	// TODO: see notebook 3. TL;DR setting abs is necessary so that we set minwidth. setting minwidth is necessary because the 
	//	// parent container will then have the correct minwidth. the parent container having the correct minwidth is necessary because 
	//	// then it'll clamp to the correct size at the end of the autosize function. this is non-ideal: for one, it takes multiple layout 
	//	// passes for this to work (one to set the min size, another to clamp to it). also, shouldn't setting width be sufficient during 
	//	// layout calculations?
	//	control->setWidthAbs(textWidth + control->paddingLeft + control->paddingRight); 
	//}

	//// if autosize disabled, then text (wrap) width depends on label width --> will be set in wrap pass
	//return;


	//if (control->isAutosize(Axis::HORIZONTAL))
	if (control->labAutosize)
	{
		// set wrap width based on max width
		int wrapWidth = std::max(control->minWidth.value_or(0), control->maxWidth.value_or(INT_MAX)); // if min > max, then use min instead (min always trumps max in UI)
		if (wrapWidth == INT_MAX) wrapWidth = 0; // note: max width is INT_MAX by default! so if the user hasn't set a max size for the label, there is effectively no wrapping. SDL_TTF happens to interpret a wrapWidth of 0 as no wrapping
		else wrapWidth -= (control->paddingLeft + control->paddingRight);
		control->tttext->setWrapWidth(wrapWidth);

		// calculate text width now that we have set wrap width
		int textWidth = control->tttext->getDimensions().first;

		// label width is can now be set to fit to text width, as per the rule of autosizing
		int controlWidth = textWidth + control->paddingLeft + control->paddingRight;
		control->setPreferredWidth(controlWidth); // TODO: see equivalent function is horizontal autosize
		control->minWidthCalculated = controlWidth;
		control->maxWidthCalculated = controlWidth;

		// min and max width take precedence over autosizing
		// this might come into play if e.g., text is very short, so we need to grow the label to at least its min width
		//control->w = MoreMath::clamp(control->w, control->getMinWidth(), control->getMaxWidth());
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
