#include "GG/UI/Controls/Combobox.h"

// bounds
void Combobox::setBounds(int _x, int _y, int _w, int _h)
{
	x = _x;
	y = _y;
	w = _w;
	h = _h;
}

// color
void Combobox::setForeColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	foreColor = { r, g, b, a };
}
void Combobox::setBackColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	backColor = { r, g, b, a };
}

// text
void Combobox::setText(std::string _text)
{
	text = _text;
}
std::string Combobox::getText() const
{
	return text;
}

void Combobox::render(Canvas *canvas)
{
	if (isInBounds)
	{
		canvas->setColor(backColor);
	}
	else
	{
		canvas->setColor(128, 128, 128);
	}
	canvas->fillRect(x, y, w, h);

	canvas->setColor(foreColor);
	canvas->setAlignment(Canvas::Alignment::CENTER_CENTER);
	canvas->drawString(text, x + w / 2, y + h / 2);
	canvas->setAlignment(Canvas::Alignment::TOP_LEFT); // TODO: it would be useful if you could push/pop canvas states, rather than having to restore values manually (esp. since the user might not have has TOP_LEFT before this call, and also since we aren't currently restoring the color)
	
	if (isExpanded)
	{
		for (int i = 0; i < options.size(); i++)
		{
			int optionY = y + (i + 1) * h;

			if (i == indexToHighlight)
				canvas->setColor(backColor);
			else
				canvas->setColor(128, 128, 128);

			canvas->fillRect(x, optionY, w, h);

			canvas->setColor(foreColor);
			canvas->setAlignment(Canvas::Alignment::CENTER_CENTER);
			canvas->drawString(options.at(i), x + w / 2, optionY + h / 2);
			canvas->setAlignment(Canvas::Alignment::TOP_LEFT); // TODO: it would be useful if you could push/pop canvas states, rather than having to restore values manually (esp. since the user might not have has TOP_LEFT before this call, and also since we aren't currently restoring the color)
		}
	}
}

// mouse events
void Combobox::onMouseEvent(MouseEventType mouseEventType, int mouseX, int mouseY)
{
	isInBounds = (mouseX >= x && mouseX < x + w) && (mouseY >= y && mouseY < y + h);
	bool isInBoundsExpanded = (mouseX >= x && mouseX < x + w) && (mouseY >= y && mouseY < y + h*(options.size()+1));


	if (mouseEventType == MouseEventType::LEFT_MOUSE_UP)
	{
		if (isExpanded && isInBoundsExpanded)
		{
			int selectedOptionIndex = (mouseY - y - h) / h;
			setText(options.at(selectedOptionIndex));
			isExpanded = false;
			
		}

		if (isInBounds)
		{
			isExpanded = !isExpanded;
		}
		else
		{
			isExpanded = false;
		}
	}

	if (mouseEventType == MouseEventType::MOUSE_MOVE)
	{
		if (isExpanded && isInBoundsExpanded)
		{
			indexToHighlight = (mouseY - y - h) / h;
		}
		else
		{
			indexToHighlight = -1;
		}
	}
}
