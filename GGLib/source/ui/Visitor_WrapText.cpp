#include "ui/Visitor.h"
#include "ui/Container.h"
#include "ui/Control.h"

void Visitor_WrapText::visitForControl(Control* control)
{
	// if autosize disabled, then text (wrap) width depends on label width --> will be set in wrap pass
	//if (!control->isAutosize(Axis::HORIZONTAL))
	if (!control->labAutosize)
	{
	
		control->tttext->setWrapWidth(control->w - control->paddingLeft - control->paddingRight);
	}
	auto newHeight = control->tttext->getDimensions().second;
	control->h = newHeight + control->paddingTop + control->paddingBottom;
	control->h = MoreMath::clamp(control->h, control->getMinHeight(), control->getMaxHeight());
}

void Visitor_WrapText::visitForFlexContainer(Container* container)
{
	for (auto child : container->children) child->accept(*this);
}

void Visitor_WrapText::visitForAbsoluteContainer(Container* container)
{
	for (auto child : container->children) child->accept(*this);
}
