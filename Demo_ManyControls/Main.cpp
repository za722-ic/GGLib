#include <fstream>
#include <iostream>
#include <string>

#include "PerformanceTracker.h"
#include "GG/Core/Application.h"

class MyApp : public GG::Application
{
private:
	GG::RootContainer* root = nullptr;

	PerformanceTracker perfTracker;

	double elapsedTimeSinceFirstLoop = 0.0f;
	const double minElapsedTimeSinceFirstLoop = 10.0f; // wait some time for transient behaviour to end and cache to be established
	const double maxElapsedTimeSinceFirstLoop = 25.0f;

	const double memorySampleInterval = 3.0f;
	double timeElapsedSinceLastMemorySample = 0.0f;

public:
	int totalElements = -1;

private:
	bool onInit() override
	{
		// set up window
		window.setTitle("MyApp");
		window.setResizable(true);
		window.maximise();
		window.setSize(1920, 1008);

		// load font
		assetManager.setBaseAssetPath("resources/");
		assetManager.loadFont("NotoSans-Medium.ttf", 10);

		// init text subsystem
		canvas.setFont(assetManager.getFont("NotoSans-Medium.ttf").value());
		GG::Text::init(canvas.getSDLRenderer(), assetManager.getFont("NotoSans-Medium.ttf").value());

		// define UI
		defineElements();

		// tell library set up was successful
		return true;
	}

	void onQuit() override
	{
		root->destroySelfAndChildren();

		auto results = perfTracker.calculateMetrics();

		std::ofstream logFile("metrics.csv", std::ios_base::app);
		logFile << totalElements << "," << results.averageFps << "," << results.onePercentLowFps << "," << results.maxMem << std::endl;

		std::cout << "Total elements = " << totalElements << std::endl;
		std::cout << "Average FPS: " << results.averageFps << std::endl;
		std::cout << "One percent low FPS: " << results.onePercentLowFps << std::endl;
		std::cout << "Peak Physical Memory Usage: " << results.maxMem << " MB" << std::endl << std::endl;
	}

	void onLoop() override
	{
		// clear screen
		canvas.setColor(0x58, 0x5B, 0x65);
		canvas.clear();

		// set title to FPS
		window.setTitle("FPS: " + std::to_string(averageFPS()));

		// layout and render UI
		root->calculateLayout(0, 0, window.getWidth(), window.getHeight());
		root->render(&canvas);

		elapsedTimeSinceFirstLoop += deltaTime();
		if (elapsedTimeSinceFirstLoop >= maxElapsedTimeSinceFirstLoop)
		{
			isRunning = false;
			return;
		}
		else if (elapsedTimeSinceFirstLoop >= minElapsedTimeSinceFirstLoop)
		{
			perfTracker.recordFrame(deltaTime());
		}

		timeElapsedSinceLastMemorySample += deltaTime();
		if (timeElapsedSinceLastMemorySample >= memorySampleInterval)
		{
			perfTracker.sampleMemory();
			timeElapsedSinceLastMemorySample = 0.0f;
		}


		// update screen
		canvas.present();
	}

private:
	void defineElements()
	{ 
		// define root container
		root = new GG::RootContainer;
		root->setInputManager(&inputManager);
		root->layoutMode = GG::LayoutMode::ABSOLUTE;
		root->verticalAlignmentMode = GG::VAlignmentMode::TOP;
		root->verticalScroll = true;
		root->isScrollEventListener = true;
		root->verticalAutosize = true;
		
		// create label and add it to the UI
		const int gap = 2;
		int x = gap, y = gap;
		const int size = 30;
		
		for (int i = 0; i < totalElements; i++)
		{
			GG::Label* label = new GG::Label(std::to_string(i));

			label->setWidthAbs(size);
			label->setHeightAbs(size);
			label->xAbs = x;
			label->yAbs = y;
			label->radius = 5;
			label->setColor({ 255,255,255,255 });
			label->setForeColor({ 0,0,0,255 });
			label->setHAlignment(GG::HAlignmentMode::CENTER);

			root->add(label);

			x += size + gap;
			if (x >= window.getWidth() - size)
			{
				x = gap;
				y += size + gap;
			}
		}
	}
};

int main(int argc, char* args[])
{
	std::ofstream logFile("metrics.csv");
	logFile << "TotalElements,AverageFPS,OnePercentLowFPS,PeakMemoryMB\n";
	logFile.close();

	for (int totalElements = 100; totalElements <= 4000; totalElements += 100)
	{
		MyApp app;
		app.totalElements = totalElements;
		app.begin();
	}

	return 0;
}
