#include "GG/UI/UI.h"
#include "GG/UI/Visitor.h"

void Visitor_DestroySelfAndChildren::visitForControl(Control *control)
{
	delete control;
}

void Visitor_DestroySelfAndChildren::visitForLabel(Label* label)
{
	visitForControl(label);
}

void Visitor_DestroySelfAndChildren::visitForFlexContainer(Container *container)
{
	for (auto child : container->children) delete child;

	delete container;
}

void Visitor_DestroySelfAndChildren::visitForAbsoluteContainer(Container* container)
{
	for (auto child : container->children) delete child;

	delete container;
}
