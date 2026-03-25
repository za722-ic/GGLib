#pragma once

#include "GGApp.h"
#include "DemoAssetManager.h"
#include "ui/Checkbox.h"
#include "ui/Button.h"
#include "ui/Slider.h"
#include "ui/Textbox.h"
#include "ui/Combobox.h"
#include "ui/Label.h"

class UI
{
private:
	std::vector<UIControl*> children;

	Canvas* canvas;
	InputManager* inputManager;

public:
	UI()
	{}

	void init(Canvas* canvas, InputManager* inputManager)
	{
		this->canvas = canvas;
		this->inputManager = inputManager;
	}

	void add(UIControl* control)
	{
		inputManager->addMouseEventListener(control);  // TODO: at present, *every* control becomes a mouselistener, even if they don't actually do anything with mouse events (unlikely, but possible) --> any easy way to optimise this? (only bother if there is lag)
		inputManager->addTextInputEventListener(control);
		inputManager->addKeyEventListener(control);
		children.push_back(control);
	}

	void remove(UIControl* control)
	{
		// TODO: what if control is not present?
		// TODO: this does NOT delete the child, which *i think* is a good idea, but you may want to revisit this later --> remove means remove it from the UI, not from existence
		inputManager->removeMouseEventListener(control);
		inputManager->removeKeyEventListener(control);
		inputManager->removeTextInputEventListener(control);
		std::erase(children, control); 
	}

	void render()
	{
		for (auto child : children) child->render(canvas);
	}

	~UI()
	{
		for (auto child : children) delete child;
	}
};

class Demo: public GGApp, KeyEventListener, MouseEventListener
{
private:
	std::shared_ptr<DemoAssetManager> demoAssetManager;

	UI ui;
	Label *lblFPS, *lblWindowSize, *lblCursorPos;

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
