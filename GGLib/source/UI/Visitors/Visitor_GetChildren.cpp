#include "GG/UI/UI.h"
#include "GG/UI/Visitors/Visitor.h"

void Visitor_GetChildren::visitForControl(Control *control)
{
	children.clear();
}

void Visitor_GetChildren::visitForLabel(Label* label)
{
	visitForControl(label);
}

void Visitor_GetChildren::visitForFlexContainer(Container *container)
{
	this->children = container->children;
}

void Visitor_GetChildren::visitForAbsoluteContainer(Container* container)
{
	this->children = container->children;
}
