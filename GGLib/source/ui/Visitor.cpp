#include "Visitor.h"
#include "Container.h"
#include "Control.h"

void Visitor::visitForContainer(Container* container)
{
	switch (container->layoutMode)
	{
	case LayoutMode::FLEX:
		visitForFlexContainer(container);
		break;
	case LayoutMode::ABSOLUTE:
		visitForAbsoluteContainer(container);
		break;

	default:
		assert(false); // unknown layout mode
	}
}
