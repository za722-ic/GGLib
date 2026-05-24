#pragma once

#include "GG/Core/Application.h"

#include "AssetManagerWrapper.h"
#include "PerlinViewer.h"

class Noise: public GG::Application, GG::KeyEventListener, GG::MouseEventListener
{
private:
	std::shared_ptr<AssetManagerWrapper> assetManagerWrapper;

	GG::Container* root;
	GG::Label *lblWindowSize, *lblCursorPos;
	GG::Label* lblPosX, * lblPosY, * lblPosZ;
	GG::Slider* sliderVelX, * sliderVelY, * sliderVelZ;
	GG::Slider* sliderOctaves, *sliderInitialFrequency, *sliderInitialAmplitude;
	GG::Slider* sliderResolutionDivision;
	GG::Toggle* cbRoundNoise;
	GG::Label* lblFPS;
	GG::Button *btnResetPosition, *btnResetVelocity;
	PerlinViewer* perlinViewer;

	float frameTimesAcc = 0.0f;
	unsigned int framesCount = 0;
	float avgFps = 0.0f;

public:
	Noise() : assetManagerWrapper(std::make_shared<AssetManagerWrapper>(&assetManager))
	{}

private:

	bool onInit() override;
	void onLoop() override;
	void onQuit() override;

	void onKeyEvent(GG::KeyEventType keyEventType, SDL_Keycode key) override;
	void onMouseEvent(GG::MouseEventType mouseEventType, int mouseX, int mouseY) override;

	void sizeButton(GG::Button* button);
	GG::Label* createLabel(std::string labelText = "-");
	GG::Slider* createSlider(float min = 0, float max = 100, float interval = 1, float startingVal = 30);
	GG::Container *createPanel(std::string panelTitle, GG::Button *btnReset, std::vector<GG::Control*> controls, std::vector<std::string> controlLabels);
	GG::Button* createResetButton();
	void defineElements();
};
