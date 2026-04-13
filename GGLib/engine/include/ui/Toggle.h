#pragma once

#include <functional>

#include "Control.h"

class Toggle : public Control
{
public:
	std::function<void()> onClick;
	std::function<void()> onMouseEnter;
	std::function<void()> onMouseExit;

	SDL_Color onBackColor= { 32, 225, 100, 255 };
	SDL_Color offBackColor= { 255, 32, 100, 255 };
	SDL_Color toggleColor = { 230,230,230,255 };
	SDL_Color hoverToggleColor= { 180,180,180,255 };

	int trianglesPerCorner = 16;

	bool isChecked = false;

	int padding;

public:
	Toggle()
	{
		radius = 10;
		setHeightAbs(radius * 2);
		setWidthAbs(radius * 4);
		padding = 2;
		shadowThickness = 4;

		setOnClick([&]() { isChecked = !isChecked;  });
	}

	// events
	void setOnClick(std::function<void()> func)
	{
		onClick = func;
	}
	void setOnMouseEnter(std::function<void()> func)
	{
		onMouseEnter = func;
	}
	void setOnMouseExit(std::function<void()> func)
	{
		onMouseExit = func;
	}

	void render(Canvas* canvas) override
	{
		// render body
		if (isChecked)
			canvas->setColor(onBackColor);
		else
			canvas->setColor(offBackColor);

		canvas->fillRoundedRect(screenX, screenY, w, h, h / 2, trianglesPerCorner);
		canvas->drawRoundedRect(screenX - shadowThickness, screenY - shadowThickness, 
			                    w + 2 * shadowThickness, h + 2 * shadowThickness, 
			                    h / 2 + shadowThickness, 
			                    shadowThickness, 
			                    { 0,0,0,96 }, { 0,0,0,0 }, 
			                    trianglesPerCorner);


		// render toggle button
		if (isInBounds)
			canvas->setColor(hoverToggleColor);
		else
			canvas->setColor(toggleColor);

		int toggleX;
		int toggleW = h - padding * 2;
		int toggleY = screenY + padding;
		if (isChecked)
			toggleX = screenX + w - toggleW - padding;
		else
			toggleX = screenX + padding;
		
		canvas->fillRoundedRect(toggleX , screenY + padding, toggleW, toggleW, h / 2 - padding, trianglesPerCorner);
		canvas->drawRoundedRect(toggleX - shadowThickness, toggleY - shadowThickness, 
			                    toggleW + 2 * shadowThickness, toggleW + 2 * shadowThickness, 
			                    toggleW / 2 + shadowThickness, 
			                    shadowThickness, 
			                    { 0,0,0,96 }, { 0,0,0,0 }, 
			                    trianglesPerCorner);
	}

private:

	// mouse events
	bool wasInBounds = false; // on prev call
	bool isInBounds = false;
	void onMouseEvent(MouseEventType mouseEventType, int mouseX, int mouseY) override
	{
		// TODO: the same logic is in button -- refactor
		wasInBounds = isInBounds;
		isInBounds = (mouseX >= screenX && mouseX < screenX + w) && (mouseY >= screenY && mouseY < screenY + h);

		if (isInBounds && mouseEventType == MouseEventType::LEFT_MOUSE_UP && onClick)
		{
			onClick();
		}

		if (!wasInBounds && isInBounds && mouseEventType == MouseEventType::MOUSE_MOVE && onMouseEnter)
		{
			onMouseEnter();
		}

		if (wasInBounds && !isInBounds && mouseEventType == MouseEventType::MOUSE_MOVE && onMouseExit)
		{
			onMouseExit();
		}
	}
};
