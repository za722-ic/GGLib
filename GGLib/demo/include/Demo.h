#pragma once

#include "GGApp.h"
#include "DemoAssetManager.h"
#include "ui/Checkbox.h"
#include "ui/Button.h"
#include "ui/Slider.h"
#include "ui/Textbox.h"
#include "ui/Combobox.h"
#include "ui/Label.h"

class Demo: public GGApp, KeyEventListener, MouseEventListener
{
private:
	std::shared_ptr<DemoAssetManager> demoAssetManager;

	Button btn0;
	Button btn1;
	Button btn2;
	Button btn3;
	Label fpsLabel;
	Label resolutionLabel;
	Label cursorPosLabel;

	float frameTimesAcc;
	unsigned int framesCount;
	float avgFps;

public:
	Demo() : demoAssetManager(std::make_shared<DemoAssetManager>(&assetManager))
	{}

private:
	void initUI();

	bool onInit() override;
	void onLoop() override;
	void onQuit() override;

	void onKeyEvent(KeyEventType keyEventType, SDL_Keycode key) override;
	void onMouseEvent(MouseEventType mouseEventType, int mouseX, int mouseY) override;
};
