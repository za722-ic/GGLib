#pragma once

#include "Control.h"

class HorizontalSpacer: public Control
{
public:
	HorizontalSpacer(int minWidth = 0)
	{
		setMinWidth(minWidth);
		setHeightAbs(0);
	}

	void render(Canvas* canvas) override
	{
	}
};
