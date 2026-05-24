#include "GG/UI/Controls/Toggle.h"

Toggle::Toggle()
{
	radius = 10;
	setHeightAbs(radius * 2);
	setWidthAbs(radius * 4);
	padding = 2;
	shadowThickness = 4;

	setOnClick([&]() { isChecked = !isChecked;  });
}

void Toggle::setOnClick(std::function<void()> func)
{
	onClick = func;
}
void Toggle::setOnMouseEnter(std::function<void()> func)
{
	onMouseEnter = func;
}
void Toggle::setOnMouseExit(std::function<void()> func)
{
	onMouseExit = func;
}

void Toggle::render(Canvas* canvas)
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

	canvas->fillRoundedRect(toggleX, screenY + padding, toggleW, toggleW, h / 2 - padding, trianglesPerCorner);
	canvas->drawRoundedRect(toggleX - shadowThickness, toggleY - shadowThickness,
		toggleW + 2 * shadowThickness, toggleW + 2 * shadowThickness,
		toggleW / 2 + shadowThickness,
		shadowThickness,
		{ 0,0,0,96 }, { 0,0,0,0 },
		trianglesPerCorner);
}

void Toggle::onMouseEvent(MouseEventType mouseEventType, int mouseX, int mouseY)
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
