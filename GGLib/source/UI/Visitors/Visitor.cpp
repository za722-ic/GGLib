#include "GG/UI/Visitors/Visitor.h"
#include "GG/UI/Container.h"
#include "GG/UI/Control.h"

void GG::Visitor::visitForContainer(Container* container)
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
