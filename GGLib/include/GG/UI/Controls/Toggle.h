#pragma once

#include <functional>

#include "GG/UI/Control.h"

namespace GG
{
	class Toggle : public Control
	{
	public:
		std::function<void()> onClick;
		std::function<void()> onMouseEnter;
		std::function<void()> onMouseExit;

		SDL_Color onBackColor = { 32, 225, 100, 255 };
		SDL_Color offBackColor = { 255, 32, 100, 255 };
		SDL_Color toggleColor = { 230,230,230,255 };
		SDL_Color hoverToggleColor = { 180,180,180,255 };

		int trianglesPerCorner = 16;

		bool isChecked = false;

		int padding;

	public:
		Toggle();

		// events
		void setOnClick(std::function<void()> func);
		void setOnMouseEnter(std::function<void()> func);
		void setOnMouseExit(std::function<void()> func);

		void render(Canvas* canvas) override;

	private:

		// mouse events
		bool wasInBounds = false; // on prev call
		bool isInBounds = false;
		void onMouseEvent(MouseEventType mouseEventType, int mouseX, int mouseY) override;
	};
}
