#pragma once

#include "GG/Core/InitsSDL.h"
#include "GG/Core/Window.h"
#include "GG/AssetManagement/AssetManager.h"
#include "GG/AssetManagement/AudioManager.h"
#include "GG/Input/InputManager.h"
#include "GG/Utilities/Timer.h"
#include "GG/Rendering/Canvas.h"
#include "GG/UI/UI.h"

namespace GG
{
	class Application
	{
	private:
		double dt;

		float dtAccumulated;
		float accumulationPeriod = 1.0f; // average FPS is calculated over this duration e.g., if this is 1.0f, then avgFPS is the average FPS over the last 1.0 seconds
		unsigned int framesAccumulated;
		float avgFPS;

	protected:
		bool isRunning = false;

		// TODO: maybe make these private and use getters? will prevent var name clashes at the expense of function name clashes
		Canvas canvas;
		InputManager inputManager;
		Window window;
		AssetManager assetManager;
		AudioManager audioManager;

	public:
		Application();
		~Application();

		void begin();

	protected:
		// -- virtual methods --
		virtual bool onInit() // can be optionally overridden
		{
			return true;
		}

		virtual void onLoop() = 0; // must be overridden

		virtual void onQuit() // can be optionally overridden
		{
		}

		double deltaTime();
		double averageFPS();
		void setFPSAveragingPeriod(double newAccumulationPeriod);

	private:
		void updateTimers(double deltaTime);

		// This is here cus Rule Of 3: https://stackoverflow.com/questions/9139103/am-i-violating-rule-of-three
		Application(Application const&) = delete;
		Application& operator=(Application const&) = delete;
	};
}