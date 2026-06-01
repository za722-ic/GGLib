#pragma once

#include <functional>

#include "GG/UI/Control.h"

namespace GG
{
	class Checkbox : public Control
	{
	public:
		bool isChecked = false;
		bool isMouseOver = false;

	public:
		Checkbox();

		void render(Canvas* canvas) override;

	private:
		// mouse events
		void onMouseUp(int mouseX, int mouseY) override;
		void onMouseEnter(int mouseX, int mouseY) override;
		void onMouseExit(int mouseX, int mouseY) override;

	};
}
