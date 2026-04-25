#pragma once

#include <functional>

#include "Button.h"

// TODO use width
class Checkbox : public Button
{
public:
	bool isChecked = false;
	bool isMouseOver = false;

public:
	
	Checkbox()
	{
		setOnClick([&]() { isChecked = !isChecked; });
		setOnMouseEnter([&]() {isMouseOver = true; });
		setOnMouseExit([&]() {isMouseOver = false; });

		setWidthAbs(h); // TODO hack
	}

	void render(Canvas* canvas) override
	{
		int x = screenX;
		int y = screenY;

		int checkedPadding = 8;
		int cbThickness = 3;

		canvas->drawRect(x, y, h, h, cbThickness);

		if (isMouseOver)
		{
			auto tmp = canvas->getColor();
			canvas->setColor(tmp.r/2, tmp.g/2, tmp.b/2);

			canvas->fillRect(x + checkedPadding, y + checkedPadding, h - 2 * checkedPadding, h - 2 * checkedPadding);

			canvas->setColor(tmp);
		}

		if (isChecked)
		{
			canvas->fillRect(x + checkedPadding, y + checkedPadding, h - 2 * checkedPadding, h - 2 * checkedPadding);
		}

		canvas->setAlignment(Canvas::Alignment::CENTER_LEFT);
		canvas->drawString(text, x + h + checkedPadding, y + h/2);
		canvas->setAlignment(Canvas::Alignment::TOP_LEFT);
	}

};
