#include "GG/UI/Controls/Combobox.h"

GG::Combobox::Combobox(std::string text, GG::RootContainer *root) : root(root)
{
	isMouseEventListener = true;
	isTextInputListener = true;
}

// bounds
void GG::Combobox::setBounds(int _x, int _y, int _w, int _h)
{
	x = _x;
	y = _y;
	w = _w;
	h = _h;
}

// color
void GG::Combobox::setForeColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	foreColor = { r, g, b, a };
}
void GG::Combobox::setBackColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	backColor = { r, g, b, a };
}

// text
void GG::Combobox::setText(std::string _text)
{
	text = _text;
}
std::string GG::Combobox::getText() const
{
	return text;
}

void GG::Combobox::addOption(std::string optionText)
{
	options.push_back(optionText);
}

void GG::Combobox::clearOptions()
{
	options.clear();
}

std::vector<std::string> GG::Combobox::getOptions()
{
	return options;
}

std::string GG::Combobox::getSelectedOption()
{
	return options.at(selectedOptionIndex);
}

void GG::Combobox::render(Canvas* canvas)
{
	// draw body
	if (isMouseDown)
	{
		canvas->setColor(backColorOnMouseDown);
	}
	else if (isInBounds)
	{
		canvas->setColor(backColorOnHover);
	}
	else
	{
		canvas->setColor(backColor);
	}
	canvas->fillRoundedRect(screenX, screenY, w, h, radius);

	// draw border and shadow
	int borderRadius = radius + borderThickness;
	int shadowRadius = borderRadius + shadowThickness;

	// TODO: should border instead be AT screenX, screenY? (this applies to a whole bunch of other elements, including the Element class's own render function)
	canvas->drawRoundedRect(screenX - borderThickness,
		screenY - borderThickness,
		w + 2 * borderThickness,
		h + 2 * borderThickness, borderRadius, borderThickness, borderColor, borderColor);

	canvas->drawRoundedRect(screenX - borderThickness - shadowThickness,
		screenY - borderThickness - shadowThickness,
		w + 2 * borderThickness + 2 * shadowThickness,
		h + 2 * borderThickness + 2 * shadowThickness,
		shadowRadius,
		shadowThickness,
		{ 0,0,0,96 }, { 0,0,0,0 });

	// draw text
	canvas->setColor(foreColor);
	canvas->setAlignment(GG::Canvas::Alignment::CENTER_CENTER);
	canvas->drawString(text, screenX + w / 2, screenY + h / 2);
	canvas->setAlignment(GG::Canvas::Alignment::TOP_LEFT); // TODO: it would be useful if you could push/pop canvas states, rather than having to restore values manually (esp. since the user might not have has TOP_LEFT before this call, and also since we aren't currently restoring the color)

	// draw arrow
	canvas->setColor({ foreColor.r, foreColor.g, foreColor.b, 128});
	const int size = 5;
	const int arrowPadding = 10;
	const float PI = 3.14159265359;
	float angle;
	if (pnlDropDownOptions == nullptr)
	{
		angle = 2 * PI * 0.25f;
	}
	else
	{
		angle = 2 * PI * 0.75f;
	}
	canvas->renderRegularPolygon(screenX + w - size - arrowPadding, screenY + h / 2, 3, angle, size);

	// update dropdown item positions in case combobox has moved
	if (pnlDropDownOptions != nullptr)
	{
		pnlDropDownOptions->x = screenX;
		pnlDropDownOptions->y = screenY + h;
	}
}
