#include "GG/UI/Controls/HorizontalDivider.h"

GG::HorizontalDivider::HorizontalDivider(int dividerThickness)
{
	setHeightAbs(dividerThickness);
}

void GG::HorizontalDivider::render(GG::Canvas* canvas)
{
	canvas->setColor(color);
	canvas->fillRect(screenX, screenY, w, h);
}
