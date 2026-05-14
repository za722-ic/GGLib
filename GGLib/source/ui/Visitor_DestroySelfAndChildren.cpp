#include "ui/Visitor.h"
#include "ui/Container.h"
#include "ui/Control.h"

void Visitor_DestroySelfAndChildren::visitForControl(Control *control)
{
	delete control;
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
