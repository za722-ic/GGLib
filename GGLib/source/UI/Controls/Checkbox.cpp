#include "GG/UI/Controls/Checkbox.h"

GG::Checkbox::Checkbox()
{
	// TODO maybe dont hardcode?
	setWidthAbs(30);
	setHeightAbs(30);

	isMouseEventListener = true;
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
void GG::Checkbox::onMouseUp(int mouseX, int mouseY)
{
	isChecked = !isChecked;
}
void GG::Checkbox::onMouseEnter(int mouseX, int mouseY)
{
	isMouseOver = true;
}
void GG::Checkbox::onMouseExit(int mouseX, int mouseY)
{
	isMouseOver = false;
}
