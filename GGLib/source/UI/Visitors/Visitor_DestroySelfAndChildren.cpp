#include "GG/UI/UI.h"
#include "GG/UI/Visitors/Visitor.h"

void GG::Visitor_DestroySelfAndChildren::visitForControl(Control* control)
{
	delete control;
}

void GG::Visitor_DestroySelfAndChildren::visitForLabel(Label* label)
{
	visitForControl(label);
}

void GG::Visitor_DestroySelfAndChildren::visitForFlexContainer(Container* container)
{
	for (auto child : container->children) child->accept(*this);

	delete container;
}

void GG::Visitor_DestroySelfAndChildren::visitForAbsoluteContainer(Container* container)
{
	for (auto child : container->children) child->accept(*this);

	delete container;
}
