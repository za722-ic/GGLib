#pragma once

#include "Game.h"
#include "DemoAssetManager.h"
#include "Button.h"

class Demo: public Game, KeyEventListener, MouseEventListener
{
private:
	std::shared_ptr<DemoAssetManager> demoAssetManager;

	std::vector<Button*> buttons; // TODO: smart pointer

public:
	Demo() : demoAssetManager(std::make_shared<DemoAssetManager>(&assetManager))
	{}

private:
	bool onGameInit() override;
	void onGameLoop() override;
	void onGameQuit() override;

	void onKeyEvent(KeyEventType keyEventType, SDL_Keycode key) override;
	void onMouseEvent(MouseEventType mouseEventType, int mouseX, int mouseY) override;
};
