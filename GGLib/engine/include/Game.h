#pragma once

#include "InitsSDL.h"
#include "GameTimer.h"
#include "GameWindow.h"
#include "InputManager.h"
#include "Canvas.h"
#include "AssetManager.h"
#include "AudioManager.h"
#include "UIManager.h"

class Game
{
private:
	double dt;

protected:
	bool isRunning = false;

	// TODO: maybe make these private and use getters? will prevent var name clashes at the expense of function name clashes
	Canvas canvas;
	InputManager inputManager;
	GameWindow gameWindow;
	AssetManager assetManager;
	AudioManager audioManager;
	UIManager uiManager;

public:
	Game();
	~Game();

	void gameLoop();

protected:
	// -- virtual methods --
	virtual bool onGameInit() // can be optionally overridden
	{
		return true;
	}

	virtual void onGameLoop() = 0; // must be overridden

	virtual void onGameQuit() // can be optionally overridden
	{}

	double deltaTime();
	
private:
	void updateTimers(double deltaTime);
	
	// This is here cus Rule Of 3: https://stackoverflow.com/questions/9139103/am-i-violating-rule-of-three
	Game(Game const&) = delete;
	Game& operator=(Game const&) = delete;
};