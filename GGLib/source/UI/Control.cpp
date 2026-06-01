#include "GG/UI/Control.h"

GG::Control::Control()
{}

void GG::Control::accept(Visitor& visitor)
{
	visitor.visitForControl(this);
}
