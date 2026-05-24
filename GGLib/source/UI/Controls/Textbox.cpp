#include "GG/UI/Controls/Textbox.h"

// bounds
void Textbox::setBounds(int _x, int _y, int _w, int _h)
{
	x = _x;
	y = _y;
	w = _w;
	h = _h;
}

// color
void Textbox::setForeColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	foreColor = { r, g, b, a };
}
void Textbox::setBackColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	backColor = { r, g, b, a };
}

// text
void Textbox::setText(std::string _text)
{
	text = _text;
}
std::string Textbox::getText() const
{
	return text;
}

// rendering
void Textbox::render(Canvas* canvas)
{
	// draw light border if focused
	if (isFocused)
	{
		canvas->setColor(255);
	}
	else
	{
		canvas->setColor(128);
	}
	canvas->drawRect(x, y, w, h, 3);

	// draw text
	unsigned int viewportW = w - 2 * PADDING;
	int cursorX;
	canvas->getTextDimensions(text.substr(0, cursorPos), &cursorX, nullptr);
	if (cursorX > viewportX + viewportW)
	{
		viewportX += cursorX - viewportX - viewportW;
	}
	else if (cursorX < viewportX)
	{
		viewportX -= viewportX - cursorX;
	}
	else if (forceShiftViewportLeft && cursorX >= viewportW)
	{
		forceShiftViewportLeft = false;
		int erasedCharWidth;
		canvas->getTextDimensions(std::to_string(erasedChar), &erasedCharWidth, nullptr);
		viewportX -= erasedCharWidth;
	}

	canvas->setColor(foreColor);
	canvas->setAlignment(Canvas::Alignment::CENTER_LEFT);
	if (!text.empty())
	{
		SDL_Texture* tbStringTexture = canvas->drawStringToTexture(text); // TODO no need to re-draw every frame --> cache it and on;y update when the text updates (set a dirty flag when text is modified, and only bother with this if that is set)
		float textureHeight, textureWidth;
		SDL_GetTextureSize(tbStringTexture, &textureWidth, &textureHeight);
		SDL_FRect viewport = { viewportX, 0, std::min((int)textureWidth - viewportX, viewportW), textureHeight };
		canvas->drawImageRegion(tbStringTexture, viewport, x + PADDING, y + h / 2); // draw a part of an image
		SDL_DestroyTexture(tbStringTexture);
	}
	canvas->setAlignment(Canvas::Alignment::TOP_LEFT); // TODO: it would be useful if you could push/pop canvas states, rather than having to restore values manually (esp. since the user might not have has TOP_LEFT before this call, and also since we aren't currently restoring the color)

	// draw cursor
	if (isFocused)
	{
		canvas->setColor(255);
		canvas->fillRect(x + cursorX + PADDING - viewportX, y + PADDING, 1, h - PADDING * 2);
	}

	//// draw debug stuff
	//canvas->drawString(text, x + PADDING - viewportX, y +2* h);
	//if (!text.empty())
	//{
	//	canvas->setColor(255, 0, 0, 60);
	//	int gx;
	//	canvas->getTextDimensions(text.substr(0, cursorPos), &gx, nullptr);
	//	int gw;
	//	std::string glyph(text.substr(cursorPos, 1));
	//	canvas->getTextDimensions(glyph, &gw, nullptr);
	//	canvas->fillRect(x + gx + PADDING, y + 2*h, gw, h);
	//}
	//else
	//{
	//	canvas->setColor(0,255,0);
	//	canvas->fillRect(x, y + 2*h, 3, 3);
	//}
	//canvas->drawString(std::to_string(cursorPos), x + PADDING - viewportX, y + 3 * h);
}

// mouse events
void Textbox::onMouseEvent(MouseEventType mouseEventType, int mouseX, int mouseY)
{
	if (mouseEventType == MouseEventType::LEFT_MOUSE_UP)
	{
		// set/unset textbox focus
		bool isInBounds = (mouseX >= x && mouseX < x + w) && (mouseY >= y && mouseY < y + h);
		isFocused = isInBounds;
	}
}

// key events
void Textbox::onKeyEvent(KeyEventType keyEventType, SDL_Keycode key)
{
	if (!isFocused) return;

	if (keyEventType == KEY_UP) return;

	if (key == SDLK_BACKSPACE) deleteCharBeforeCursorPos();
	if (key == SDLK_DELETE) deleteCharAtCursorPos();
	if (key == SDLK_LEFT) moveCursorLeft();
	if (key == SDLK_RIGHT) moveCursorRight();
	if (key == SDLK_HOME) moveCursorToStartOfText();
	if (key == SDLK_END) moveCursorToEndOfText();

}

void Textbox::onTextInputEvent(std::string text)
{
	if (!isFocused) return;

	insertTextAtCursorPos(text);
}

// handling text
void Textbox::insertTextAtCursorPos(std::string t)
{
	text.insert(cursorPos, t);
	cursorPos += t.size();
}

void Textbox::deleteCharAtCursorPos()
{
	if (cursorPos >= text.size()) return; // there are no characters to delete to the right of the cursor

	text.erase(cursorPos, 1);
}

void Textbox::deleteCharBeforeCursorPos()
{
	if (cursorPos == 0) return; // no characters to the left of the cursor

	forceShiftViewportLeft = true;
	erasedChar = text.at(cursorPos - 1);
	text.erase(cursorPos - 1, 1);
	cursorPos--;
}

void Textbox::moveCursorLeft()
{
	if (cursorPos == 0) return;
	cursorPos--;
}

void Textbox::moveCursorRight()
{
	if (cursorPos == text.size()) return;
	cursorPos++;
}

void Textbox::moveCursorToStartOfPrevToken()
{
}

void Textbox::moveCursorToStartOfNextToken()
{
}

void Textbox::moveCursorToStartOfText()
{
	cursorPos = 0;
}

void Textbox::moveCursorToEndOfText()
{
	cursorPos = text.size();
}
