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
		std::function<void()> onClick;
		std::function<void()> onMouseEnter;
		std::function<void()> onMouseExit;

	public:
		Checkbox();

		// button events
		void setOnClick(std::function<void()> func);
		void setOnMouseEnter(std::function<void()> func);
		void setOnMouseExit(std::function<void()> func);

		void render(Canvas* canvas) override;

	private:

		// mouse events
		// TODO copy pasted from button --> refactor?
		bool wasInBounds = false; // on prev call
		bool isInBounds = false;
		bool isMouseDown = false;
		void onMouseEvent(MouseEventType mouseEventType, int mouseX, int mouseY) override;
	};
}
