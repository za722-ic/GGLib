#pragma once

#include "GGApp.h"
#include "NoiseAssetManager.h"
#include "ui/Checkbox.h"
#include "ui/Button.h"
#include "ui/Slider.h"
#include "ui/Textbox.h"
#include "ui/Combobox.h"
#include "ui/Label.h"
#include "ui/Container.h"
#include "ui/HorizontalSpacer.h"
#include "ui/HorizontalDivider.h"
#include "ui/Toggle.h"
#include "ui/Scrollable.h"

#include "PerlinViewer.h"

class Noise: public GGApp, KeyEventListener, MouseEventListener
{
private:
	std::shared_ptr<NoiseAssetManager> noiseAssetManager;

	Container* root;
	Label *lblWindowSize, *lblCursorPos;
	Label* lblPosX, * lblPosY, * lblPosZ;
	Slider* sliderVelX, * sliderVelY, * sliderVelZ;
	Slider* sliderOctaves, *sliderInitialFrequency, *sliderInitialAmplitude;
	Slider* sliderResolutionDivision;
	Toggle* cbRenderScale, *cbRoundNoise;
	Label* lblFPS;
	Button *btnResetPosition, *btnResetVelocity;


	float frameTimesAcc;
	unsigned int framesCount;
	float avgFps;

public:
	Noise() : noiseAssetManager(std::make_shared<NoiseAssetManager>(&assetManager))
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
	Slider* createSlider(int min = 0, int max = 100, int interval = 1, int startingVal = 30);
	Container *createPanel(std::string panelTitle, Button *btnReset, std::vector<Control*> controls, std::vector<std::string> controlLabels);
	Button* createResetButton();
	void defineElements();
};
