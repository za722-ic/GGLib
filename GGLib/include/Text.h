#pragma once

#include <SDL3_ttf/SDL_ttf.h>

#include <string>
#include <iostream>

class Text
{
private:
	// TODO: allow multiple fonts?
	static TTF_TextEngine* engine;
	static TTF_Font* font;

	TTF_Text* ttfText = nullptr;
	TTF_HorizontalAlignment horizontalAlignment = TTF_HORIZONTAL_ALIGN_LEFT;

	int x = 0;
	int y = 0;

	int w = 0;
	int h = 0;

	std::string strText;

public:
	static void init(SDL_Renderer* renderer, TTF_Font* font);
	static void close();

	Text(std::string text);
	~Text();

	void setColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	SDL_Color getColor();

	void setText(std::string text);
	std::string getText();

	void setWrapWidth(int wrapWidth);
	int getWrapWidth();

	void setPosition(int x, int y);
	std::pair<int, int> getPosition();

	void setAlignment(TTF_HorizontalAlignment alignment);
	TTF_HorizontalAlignment getAlignment();

	std::pair<int, int> getDimensions();

	void render();

private:
	void calculateDimensions();
};
