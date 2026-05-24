#pragma once

#include "GG/UI/Control.h"

class HorizontalSpacer: public Control
{
public:
	HorizontalSpacer(int minWidth = 0);

	void render(Canvas* canvas) override;
};
