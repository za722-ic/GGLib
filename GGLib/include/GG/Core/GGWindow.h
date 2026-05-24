#pragma once

#include <SDL3/SDL.h>

#include <iostream>
#include <string>
#include <optional>

class GGWindow 
{
private:
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
 
public:
	std::optional<SDL_Renderer*> init();
	void close();

	void setTitle(std::string title);

	void setFullScreen(bool isFullScreen);
	bool getFullScreen();
	void toggleFullScreen();

	void setResizable(bool isResizable);

	void setMouseHidden(bool isMouseHidden);

	void setSize(unsigned int width, unsigned int height);
	void setMinimumSize(unsigned int width, unsigned int height);

	void setPos(int x, int y);
	void centerWindowPosition();

	unsigned int getWidth();
	unsigned int getHeight();

	SDL_Window* getSDLWindow();



private:
	std::optional<SDL_Window*> createSDLWindow(unsigned int width, unsigned int height, bool isFullScreen, std::string title);
};
