#pragma once

#include "Control.h"

class HorizontalSpacer: public Control
{
public:
	HorizontalSpacer(int minWidth = 0)
	{
		setMinWidth(minWidth);
		setHeightAbs(1);
	}

	void render(Canvas* canvas) override
	{
		canvas->setColor(255, 0, 0, 128);
		canvas->fillRect(screenX, screenY, w, h);
	}
};
