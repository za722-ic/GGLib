#include "GG/UI/UI.h"
#include "GG/UI/Visitors/Visitor.h"

void GG::Visitor_WrapText::visitForControl(Control* control)
{
}

void GG::Visitor_WrapText::visitForLabel(Label* label)
{
	// if autosize disabled, then text width (wrapWidth) depends on label width
	if (!label->isAutosize(Axis::HORIZONTAL))
	{
		label->tttext->setWrapWidth(label->w - label->paddingLeft - label->paddingRight);
	}
}

void GG::Visitor_WrapText::visitForFlexContainer(Container* container)
{
	for (auto child : container->children) child->accept(*this);
}

void GG::Visitor_WrapText::visitForAbsoluteContainer(Container* container)
{
	for (auto child : container->children) child->accept(*this);
}
