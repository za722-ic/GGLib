#pragma once

#include "GG/Core/Application.h"
#include "AssetManagerWrapper.h"

class Demo: public GG::Application, GG::KeyEventListener, GG::MouseEventListener
{
private:
	std::shared_ptr<AssetManagerWrapper> demoAssetManager;

	GG::Label *lblFPS, *lblWindowSize, *lblCursorPos;
	GG::Container* root;

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

	void onKeyEvent(GG::KeyEventType keyEventType, SDL_Keycode key) override;
	void onMouseEvent(GG::MouseEventType mouseEventType, int mouseX, int mouseY) override;

	void defineElements();
};
