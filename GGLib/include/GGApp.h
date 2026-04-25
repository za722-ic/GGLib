#pragma once

#include "InitsSDL.h"
#include "Timer.h"
#include "GGWindow.h"
#include "InputManager.h"
#include "Canvas.h"
#include "AssetManager.h"
#include "AudioManager.h"
#include "UI.h"

class GGApp
{
private:
	double dt;

protected:
	bool isRunning = false;

	// TODO: maybe make these private and use getters? will prevent var name clashes at the expense of function name clashes
	Canvas canvas;
	InputManager inputManager;
	GGWindow ggWindow;
	AssetManager assetManager;
	AudioManager audioManager;

public:
	GGApp();
	~GGApp();

	void mainLoop();

protected:
	// -- virtual methods --
	virtual bool onInit() // can be optionally overridden
	{
		return true;
	}

	virtual void onLoop() = 0; // must be overridden

	virtual void onQuit() // can be optionally overridden
	{}

	double deltaTime();
	
private:
	void updateTimers(double deltaTime);
	
	// This is here cus Rule Of 3: https://stackoverflow.com/questions/9139103/am-i-violating-rule-of-three
	GGApp(GGApp const&) = delete;
	GGApp& operator=(GGApp const&) = delete;
};