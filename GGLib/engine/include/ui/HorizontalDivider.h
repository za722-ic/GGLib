#pragma once

#include "Control.h"

class HorizontalDivider: public Control
{
public:
	HorizontalDivider(int dividerThickness)
	{
		setHeightAbs(dividerThickness);
	}

	void render(Canvas* canvas) override
	{
		canvas->setColor(color);
		canvas->fillRect(screenX, screenY, w, h);
	}
};
