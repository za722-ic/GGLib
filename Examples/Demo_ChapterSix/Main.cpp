#pragma once

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
	int targetDepth = -1;

private:
	bool onInit() override
	{
		// set up window
		window.setTitle("BENCHMARK");
		window.setResizable(true);
		window.setSize(300, 300);

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
		logFile << totalElements << "," << targetDepth << "," << results.averageFps << "," << results.onePercentLowFps << "," << results.maxMem << std::endl;

		std::cout << "Total elements = " << totalElements << ", Target depth = " << targetDepth << std::endl;
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
		root = defineBenchmarkHierarchy(totalElements, targetDepth);

		root->setInputManager(&inputManager);
	}

	GG::RootContainer* defineBenchmarkHierarchy(int totalElements, int targetDepth)
	{
		// tree requires at least a root and a leaf layer (depth >= 2)
		if (targetDepth < 2 || totalElements < targetDepth)	return nullptr;

		const int globalGap = 4;

		// color palette for layers (we cycle between these)
		std::vector<SDL_Color> layer_colors =
		{
			{255, 0, 0, 255},   
			{0, 255, 0, 255},   
			{0, 0, 255, 255},   
			{255, 255, 0, 255}, 
			{0, 255, 255, 255}, 
			{255, 0, 255, 255}  
		};

		GG::RootContainer* root = new GG::RootContainer();
		root->layoutDirection = GG::LayoutDirection::TOP_TO_BOTTOM;
		root->verticalAlignmentMode = GG::VAlignmentMode::TOP;
		root->verticalScroll = true;
		root->isScrollEventListener = true;
		root->setPadding(globalGap);
		root->setChildGap(globalGap);
		root->color = layer_colors[0];
		int controls_created = 1;
		if (controls_created == totalElements) return root;

		std::vector<std::vector<GG::Container*>> layers(targetDepth - 1);
		layers[0].push_back(root);

		const int branching_factor = 3;

		for (int d = 1; d < targetDepth - 1; d++) 
		{
			for (GG::Container* parent : layers[d - 1]) 
			{
				for (int b = 0; b < branching_factor; b++) 
				{
					if (controls_created >= totalElements) break;

					GG::Container* child_container = new GG::Container();
					child_container->setChildGap(globalGap);
					child_container->setPadding(globalGap);
					child_container->color = layer_colors[d % layer_colors.size()];

					// alternating layout direction so that elements are arranged in a grid
					if (parent->layoutDirection == GG::LayoutDirection::TOP_TO_BOTTOM)
					{
						child_container->layoutDirection = GG::LayoutDirection::LEFT_TO_RIGHT;
					}
					else 
					{
						child_container->layoutDirection = GG::LayoutDirection::TOP_TO_BOTTOM;
					}

					parent->add(child_container);
					layers[d].push_back(child_container);
					controls_created++;
				}
				if (controls_created >= totalElements) break;
			}
		}

		// distribute all remaining leaf controls evenly across the deepest layer containers
		int deepest_container_layer_idx = targetDepth - 2; 

		while (controls_created < totalElements) 
		{
			bool control_allocated_this_pass = false;

			for (GG::Container* target_container : layers[deepest_container_layer_idx]) 
			{
				if (controls_created >= totalElements) break;

				GG::Control* leaf = new GG::Control();
				leaf->color = { 0, 0, 0, 255 }; // leaf controls are black
				leaf->setMinWidth(10);
				leaf->setMinHeight(10);

				target_container->add(leaf);
				controls_created++;
				control_allocated_this_pass = true;
			}

			if (!control_allocated_this_pass) break;
		}

		return root;
	}
};

int main(int argc, char* args[])
{
	std::ofstream logFile("metrics.csv");
	logFile << "TotalElements,TargetDepth,AverageFPS,OnePercentLowFPS,PeakMemoryMB\n";
	logFile.close();

	for (int targetDepth = 2; targetDepth <= 9; targetDepth++)
	{
		for (int totalElements = 100; totalElements <= 4000; totalElements += 100)
		{
			MyApp app;
			app.targetDepth = targetDepth;
			app.totalElements =  totalElements;
			app.begin();
		}
	}

	return 0;
}