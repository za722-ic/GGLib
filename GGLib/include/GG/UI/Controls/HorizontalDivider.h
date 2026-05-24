#pragma once

#include "GG/UI/Control.h"

namespace GG
{
	class HorizontalDivider : public Control
	{
	public:
		HorizontalDivider(int dividerThickness);

		void render(Canvas* canvas) override;
	};
}
