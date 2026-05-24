#pragma once

#include "GG/Core/GGApp.h"

#include "AssetManagerWrapper.h"
#include "PerlinViewer.h"

class Noise: public GGApp, KeyEventListener, MouseEventListener
{
private:
	std::shared_ptr<AssetManagerWrapper> assetManagerWrapper;

	Container* root;
	Label *lblWindowSize, *lblCursorPos;
	Label* lblPosX, * lblPosY, * lblPosZ;
	Slider* sliderVelX, * sliderVelY, * sliderVelZ;
	Slider* sliderOctaves, *sliderInitialFrequency, *sliderInitialAmplitude;
	Slider* sliderResolutionDivision;
	Toggle* cbRoundNoise;
	Label* lblFPS;
	Button *btnResetPosition, *btnResetVelocity;
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

	void onKeyEvent(KeyEventType keyEventType, SDL_Keycode key) override;
	void onMouseEvent(MouseEventType mouseEventType, int mouseX, int mouseY) override;

	void sizeLabel(Label* label);
	void sizeButton(Button* button);
	Label* createLabel(std::string labelText = "-");
	Slider* createSlider(float min = 0, float max = 100, float interval = 1, float startingVal = 30);
	Container *createPanel(std::string panelTitle, Button *btnReset, std::vector<Control*> controls, std::vector<std::string> controlLabels);
	Button* createResetButton();
	void defineElements();
};
