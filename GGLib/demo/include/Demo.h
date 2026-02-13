#pragma once

#include "Game.h"
#include "DemoAssetManager.h"
#include "ui/Checkbox.h"
#include "ui/Button.h"
#include "ui/Slider.h"
#include "ui/Textbox.h"
#include "ui/Combobox.h"

class Demo: public Game, KeyEventListener, MouseEventListener
{
private:
	std::shared_ptr<DemoAssetManager> demoAssetManager;

	std::vector<Button*> buttons; // TODO: smart pointer
	Checkbox cb;
	Slider slider;
	Textbox tb;
	Combobox combobox;

	float frameTimesAcc;
	unsigned int framesCount;
	float avgFps;

	float y = 10;
	float vy = 200;

	float triRotAngle;

	DragManager dragManager;


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
