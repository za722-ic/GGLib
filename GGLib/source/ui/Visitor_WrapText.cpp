#include "UI.h"
#include "ui/Visitor.h"

void Visitor_WrapText::visitForControl(Control* control)
{
}

void Visitor_WrapText::visitForLabel(Label* label)
{
	// if autosize disabled, then text width (wrapWidth) depends on label width
	if (!label->isAutosize(Axis::HORIZONTAL))
	{
		label->tttext->setWrapWidth(label->w - label->paddingLeft - label->paddingRight);
	}
}

void Visitor_WrapText::visitForFlexContainer(Container* container)
{
	for (auto child : container->children) child->accept(*this);
}

void Visitor_WrapText::visitForAbsoluteContainer(Container* container)
{
	for (auto child : container->children) child->accept(*this);
}
