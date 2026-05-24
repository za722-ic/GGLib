#include "GG/UI/Controls/HorizontalSpacer.h"

GG::HorizontalSpacer::HorizontalSpacer(int minWidth)
{
	setMinWidth(minWidth);
	setHeightAbs(0);
}

void GG::HorizontalSpacer::render(GG::Canvas* canvas)
{
}
