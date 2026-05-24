#pragma once

#include "GG/UI/Control.h"

namespace GG
{
	class HorizontalSpacer : public Control
	{
	public:
		HorizontalSpacer(int minWidth = 0);

		void render(Canvas* canvas) override;
	};
}
