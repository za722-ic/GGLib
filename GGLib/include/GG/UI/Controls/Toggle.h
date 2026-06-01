#pragma once

#include <functional>

#include "GG/UI/Control.h"

namespace GG
{
	class Toggle : public Control
	{
	public:
		SDL_Color onBackColor = { 32, 225, 100, 255 };
		SDL_Color offBackColor = { 255, 32, 100, 255 };
		SDL_Color toggleColor = { 230,230,230,255 };
		SDL_Color hoverToggleColor = { 180,180,180,255 };

		int trianglesPerCorner = 16;

		bool isChecked = false;

		bool isMouseHovering = false;

		int padding;

	public:
		Toggle();

		void render(Canvas* canvas) override;

	private:

		// mouse events
		void onMouseUp(int mouseX, int mouseY);
		void onMouseEnter(int mouseX, int mouseY);
		void onMouseExit(int mouseX, int mouseY);
	};
}
