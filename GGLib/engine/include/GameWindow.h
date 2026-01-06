#pragma once

#include <SDL.h>

#include <iostream>
#include <string>
#include <optional>

// TODO: make this have any resolution/size

class GameWindow 
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



private:
	bool setRenderingResolution(int screenWidth, int screenHeight);

	std::optional<SDL_Window*> createSDLWindow(int width, int height, bool isFullScreen, std::string title);
};
