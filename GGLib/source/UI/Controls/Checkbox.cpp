#include "GG/UI/Controls/Checkbox.h"

GG::Checkbox::Checkbox()
{
	setOnClick([&]() { isChecked = !isChecked; });
	setOnMouseEnter([&]() {isMouseOver = true; });
	setOnMouseExit([&]() {isMouseOver = false; });

	setWidthAbs(h); // TODO hack
}

void GG::Checkbox::render(GG::Canvas* canvas)
{
	int x = screenX;
	int y = screenY;

	int checkedPadding = 8;
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

	canvas->setAlignment(GG::Canvas::Alignment::CENTER_LEFT);
	canvas->drawString(text, x + h + checkedPadding, y + h / 2);
	canvas->setAlignment(GG::Canvas::Alignment::TOP_LEFT);
}
