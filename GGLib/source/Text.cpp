#include "Text.h"

TTF_TextEngine* Text::engine = nullptr;
TTF_Font* Text::font = nullptr;

void Text::init(SDL_Renderer* renderer, TTF_Font* font)
{
	Text::font = font;

	engine = TTF_CreateRendererTextEngine(renderer);

	if (engine == nullptr)
	{
		std::cout << "Failed to init TTF_TextEngine: " << SDL_GetError() << std::endl;
	}
}
void Text::close()
{
	// all text objects should be destroyed before calling close
	TTF_DestroyRendererTextEngine(engine);
}

Text::Text(std::string text) : strText(text)
{
	ttfText = TTF_CreateText(engine, font, strText.c_str(), 0);

	calculateDimensions();
}

Text::~Text()
{
	TTF_DestroyRendererTextEngine(engine);
}

void Text::setColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	TTF_SetTextColor(ttfText, r, g, b, a);
}
SDL_Color Text::getColor()
{
	unsigned char r, g, b, a;
	TTF_GetTextColor(ttfText, &r, &g, &b, &a);
	SDL_Color color = { r,g,b,a };
	return color;
}

void Text::setText(std::string text)
{
	strText = text;

	TTF_SetTextString(ttfText, strText.c_str(), strText.size());

	calculateDimensions();
}
std::string Text::getText()
{
	return strText;
}

void Text::setWrapWidth(int wrapWidth)
{
	TTF_SetTextWrapWidth(ttfText, wrapWidth);

	calculateDimensions();
}
int Text::getWrapWidth()
{
	int wrapW;
	TTF_GetTextWrapWidth(ttfText, &wrapW);
	return wrapW;
}

void Text::setPosition(int x, int y)
{
	this->x = x;
	this->y = y;
}
std::pair<int, int> Text::getPosition()
{
	return std::pair<int, int>(x, y);
}

void Text::setAlignment(TTF_HorizontalAlignment alignment)
{
	TTF_Font *font = TTF_GetTextFont(ttfText);
	TTF_SetFontWrapAlignment(font, alignment);
}
TTF_HorizontalAlignment Text::getAlignment()
{
	return TTF_GetFontWrapAlignment(font);
}

std::pair<int, int> Text::getDimensions()
{
	return std::pair<int, int>(w, h);
}

void Text::render()
{
	TTF_DrawRendererText(ttfText, x, y);
}



void Text::calculateDimensions()
{
	TTF_GetStringSizeWrapped(font, strText.c_str(), strText.size(), getWrapWidth(), &w, &h);
}

