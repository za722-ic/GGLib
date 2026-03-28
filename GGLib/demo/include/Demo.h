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
	GGWindow* window;

public:
	UI()
	{}

	void init(Canvas* canvas, InputManager* inputManager, GGWindow* window)
	{
		this->canvas = canvas;
		this->inputManager = inputManager;
		this->window = window;
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
		calculateLayout(); // TODO: instead, only do this when the window is resized (at least by default, with the option from the user to do this manually too i.e, make calculateLayout public in the future maybe)

		canvas->setAlignment(Canvas::Alignment::TOP_LEFT); // TODO: it would be useful if you could push/pop canvas states, rather than having to restore values manually (esp. since the user might not have has TOP_LEFT before this call, and also since we aren't currently restoring the color)

		for (auto child : children) child->render(canvas);
	}

	~UI()
	{
		for (auto child : children) delete child;
	}

private:
	
	enum class LayoutType
	{
		GRID	
	};

	LayoutType layoutType = LayoutType::GRID;

	void calculateLayout()
	{
//		switch (layoutType)
//		{
//		case LayoutType::GRID: 
//			calculateGridLayout(); break;
//		default: 
//			calculateFreeLayout(); break;
//		}
		calculateGridLayout();
	}

	void calculateGridLayout()
	{
		// TODO: for now im just sizing them equally and arranging them in a single column --> need to have adjustable ratios, adjustable cell locations
		
		// calculate size of child elements
		int padding = 50;
		unsigned int sizePerElement = window->getHeight() / children.size();
		for (int i = 0; i < children.size(); i++)
		{
			children.at(i)->y = sizePerElement * i;
			children.at(i)->h = sizePerElement;
			children.at(i)->w = window->getWidth();
			children.at(i)->x = 0;
		}
	}
	void calculateFreeLayout()
	{
		// manually position elements, manually size elements	
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
