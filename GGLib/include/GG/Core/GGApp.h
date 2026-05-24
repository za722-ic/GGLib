#pragma once

#include "GG/Core/InitsSDL.h"
#include "GG/Core/GGWindow.h"
#include "GG/AssetManagement/AssetManager.h"
#include "GG/AssetManagement/AudioManager.h"
#include "GG/Input/InputManager.h"
#include "GG/Utilities/Timer.h"
#include "GG/Rendering/Canvas.h"
#include "GG/UI/UI.h"

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