#include "GG/UI/Controls/Checkbox.h"

GG::Checkbox::Checkbox()
{
	setOnClick([&]() { isChecked = !isChecked; });
	setOnMouseEnter([&]() {isMouseOver = true; });
	setOnMouseExit([&]() {isMouseOver = false; });

	// TODO maybe dont hardcode?
	setWidthAbs(30);
	setHeightAbs(30);
}

void GG::Checkbox::setOnClick(std::function<void()> func)
{
	onClick = func;
}
void GG::Checkbox::setOnMouseEnter(std::function<void()> func)
{
	onMouseEnter = func;
}
void GG::Checkbox::setOnMouseExit(std::function<void()> func)
{
	onMouseExit = func;
}

void GG::Checkbox::render(GG::Canvas* canvas)
{
	int x = screenX;
	int y = screenY;

	int checkedPadding = 3;
	int cbThickness = 3;

	canvas->drawRect(x, y, h, h, cbThickness);

	if (isMouseOver)
	{
		auto tmp = canvas->getColor();
		canvas->setColor(tmp.r / 2, tmp.g / 2, tmp.b / 2);

		canvas->fillRect(x + checkedPadding, y + checkedPadding, h - 2 * checkedPadding, h - 2 * checkedPadding);

		canvas->setColor(tmp);
	}

	if (isChecked)
	{
		canvas->fillRect(x + checkedPadding, y + checkedPadding, h - 2 * checkedPadding, h - 2 * checkedPadding);
	}
}

// mouse events
void GG::Checkbox::onMouseEvent(GG::MouseEventType mouseEventType, int mouseX, int mouseY)
{
	wasInBounds = isInBounds;
	isInBounds = (mouseX >= screenX && mouseX < screenX + w) && (mouseY >= screenY && mouseY < screenY + h);


	if (isInBounds)
	{
		if (mouseEventType == GG::MouseEventType::LEFT_MOUSE_DOWN)
		{
			isMouseDown = true;
		}
		else if (mouseEventType == GG::MouseEventType::LEFT_MOUSE_UP)
		{
			isMouseDown = false;
			if (onClick) onClick();
		}
	}
	else
	{
		isMouseDown = false;
	}

	if (!wasInBounds && isInBounds && mouseEventType == GG::MouseEventType::MOUSE_MOVE && onMouseEnter)
	{
		onMouseEnter();
	}

	if (wasInBounds && !isInBounds && mouseEventType == GG::MouseEventType::MOUSE_MOVE && onMouseExit)
	{
		onMouseExit();
	}
}
