#pragma once

#include "GGApp.h"
#include "AssetManagerWrapper.h"

class Demo: public GGApp, KeyEventListener, MouseEventListener
{
private:
	std::shared_ptr<AssetManagerWrapper> demoAssetManager;

	Label *lblFPS, *lblWindowSize, *lblCursorPos;
	Container* root;

	float frameTimesAcc;
	unsigned int framesCount;
	float avgFps;

public:
	Demo() : demoAssetManager(std::make_shared<AssetManagerWrapper>(&assetManager))
	{}

private:

	bool onInit() override;
	void onLoop() override;
	void onQuit() override;

	void onKeyEvent(KeyEventType keyEventType, SDL_Keycode key) override;
	void onMouseEvent(MouseEventType mouseEventType, int mouseX, int mouseY) override;

	void defineElements();
};
