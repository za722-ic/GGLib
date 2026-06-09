#include "GG/Rendering/Text.h"

TTF_TextEngine* GG::Text::engine = nullptr;
TTF_Font* GG::Text::font = nullptr;

void GG::Text::init(SDL_Renderer* renderer, TTF_Font* font)
{
	GG::Text::font = font;

	engine = TTF_CreateRendererTextEngine(renderer);

	if (engine == nullptr)
	{
		std::cout << "Failed to init TTF_TextEngine: " << SDL_GetError() << std::endl;
	}
}
void GG::Text::close()
{
	// all text objects should be destroyed before calling close
	TTF_DestroyRendererTextEngine(engine);
}

GG::Text::Text(std::string text) : strText(text)
{
	ttfText = TTF_CreateText(engine, font, strText.c_str(), 0);

	calculateDimensions();
}

GG::Text::~Text()
{
	TTF_DestroyRendererTextEngine(engine);
}

void GG::Text::setColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	TTF_SetTextColor(ttfText, r, g, b, a);
}
SDL_Color GG::Text::getColor()
{
	unsigned char r, g, b, a;
	TTF_GetTextColor(ttfText, &r, &g, &b, &a);
	SDL_Color color = { r,g,b,a };
	return color;
}

void GG::Text::setText(std::string text)
{
	strText = text;

	TTF_SetTextString(ttfText, strText.c_str(), strText.size());

	calculateDimensions();
}
std::string GG::Text::getText()
{
	return strText;
}

void GG::Text::setWrapWidth(int newWrapWidth)
{
	// only bother if wrap width changed
	if (getWrapWidth() == newWrapWidth)
		return;

	TTF_SetTextWrapWidth(ttfText, newWrapWidth);

	calculateDimensions();
}
int GG::Text::getWrapWidth()
{
	int wrapW;
	TTF_GetTextWrapWidth(ttfText, &wrapW);
	return wrapW;
}

void GG::Text::setPosition(int x, int y)
{
	this->x = x;
	this->y = y;
}
std::pair<int, int> GG::Text::getPosition()
{
	return std::pair<int, int>(x, y);
}

void GG::Text::setHAlignment(HAlignmentMode alignment)
{
	TTF_Font* font = TTF_GetTextFont(ttfText);
	TTF_SetFontWrapAlignment(font, hAlignmentModeToTTFHorizontalAlignment(alignment));
}
GG::HAlignmentMode GG::Text::getHAlignment()
{
	return TTFHorizontalAlignmentToHAlignmentMode(TTF_GetFontWrapAlignment(font));
}

std::pair<int, int> GG::Text::getDimensions()
{
	return std::pair<int, int>(w, h);
}

void GG::Text::render()
{
	TTF_DrawRendererText(ttfText, x, y);
}

void GG::Text::calculateDimensions()
{
	TTF_GetStringSizeWrapped(font, strText.c_str(), strText.size(), getWrapWidth(), &w, &h);
}

TTF_HorizontalAlignment GG::Text::hAlignmentModeToTTFHorizontalAlignment(HAlignmentMode horizontalAlignment)
{
	switch (horizontalAlignment)
	{
	case HAlignmentMode::LEFT:
		return TTF_HORIZONTAL_ALIGN_LEFT;

	case HAlignmentMode::CENTER:
		return TTF_HORIZONTAL_ALIGN_CENTER;

	case HAlignmentMode::RIGHT:
		return TTF_HORIZONTAL_ALIGN_RIGHT;

	default:
		assert(false); // something wrong has happened
	}
}

GG::HAlignmentMode GG::Text::TTFHorizontalAlignmentToHAlignmentMode(TTF_HorizontalAlignment horizontalAlignment)
{
	switch (horizontalAlignment)
	{
	case TTF_HORIZONTAL_ALIGN_LEFT:
		return HAlignmentMode::LEFT;

	case TTF_HORIZONTAL_ALIGN_CENTER:
		return HAlignmentMode::CENTER;

	case TTF_HORIZONTAL_ALIGN_RIGHT:
		return HAlignmentMode::RIGHT;

	default:
		assert(false); // something wrong has happened
	}
}
