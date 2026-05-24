#include "GG/UI/UI.h"
#include "GG/UI/Visitors/Visitor.h"

void GG::Visitor_GetChildren::visitForControl(Control* control)
{
	children.clear();
}

void GG::Visitor_GetChildren::visitForLabel(Label* label)
{
	visitForControl(label);
}

void GG::Visitor_GetChildren::visitForFlexContainer(Container* container)
{
	this->children = container->children;
}

void GG::Visitor_GetChildren::visitForAbsoluteContainer(Container* container)
{
	this->children = container->children;
}
