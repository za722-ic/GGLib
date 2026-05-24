#include "GG/UI/Controls/HorizontalDivider.h"

HorizontalDivider::HorizontalDivider(int dividerThickness)
{
	setHeightAbs(dividerThickness);
}

void HorizontalDivider::render(Canvas* canvas)
{
	canvas->setColor(color);
	canvas->fillRect(screenX, screenY, w, h);
}
