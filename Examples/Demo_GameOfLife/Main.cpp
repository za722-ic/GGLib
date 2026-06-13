#include "GG/Core/Application.h"

#include "GoLControl.h"

class MyApp : public GG::Application
{
private:
	GG::RootContainer* root = nullptr;

private:
	bool onInit() override
	{
		// set up window
		window.setTitle("Conway's Game of Life");
		window.setResizable(true);
		window.setSize(300, 300);

		// load font
		assetManager.setBaseAssetPath("resources/");
		assetManager.loadFont("NotoSans-Medium.ttf", 13);

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
		GG::Text::close();
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

		// update screen
		canvas.present();
	}

private:
	void defineElements()
	{
		// define root container
		root = new GG::RootContainer;
		root->setInputManager(&inputManager);
		root->layoutDirection = GG::LayoutDirection::TOP_TO_BOTTOM;

		// create game of life custom control
		GoLControl* golControl = new GoLControl();
		root->add(golControl);

		// create settings panel
		GG::Container* pnlSettings = new GG::Container;
		pnlSettings->layoutDirection = GG::LayoutDirection::LEFT_TO_RIGHT;
		pnlSettings->setChildGap(20);
		pnlSettings->setPadding(20);
		pnlSettings->setColor({ 30,30,30,255 });
		pnlSettings->verticalAutosize = true;
		pnlSettings->shadowThickness = 8;
		pnlSettings->radius = 0;
		root->add(pnlSettings);
		
		// create controls for settings
		GG::Slider *sliderSimulationSpeed = new GG::Slider;
		sliderSimulationSpeed->setMaxWidth(400);
		pnlSettings->add(sliderSimulationSpeed);

		GG::Toggle* togglePaused = new GG::Toggle;
		pnlSettings->add(togglePaused);

		GG::Toggle* toggleRenderCoords = new GG::Toggle;
		pnlSettings->add(toggleRenderCoords);

		GG::Button* btnReturnToOrigin = new GG::Button("Return to origin");
		btnReturnToOrigin->setHeightAbs(30);
		btnReturnToOrigin->setWidthAbs(200);
		btnReturnToOrigin->radius = 3;
		pnlSettings->add(btnReturnToOrigin);

		GG::Button* btnClearCells = new GG::Button("Clear cells");
		btnClearCells->setHeightAbs(30);
		btnClearCells->setWidthAbs(200);
		btnClearCells->radius = 3;
		pnlSettings->add(btnClearCells);

		GG::Textbox *tbRuleString = new GG::Textbox;
		tbRuleString->setHeightAbs(30);
		tbRuleString->setWidthAbs(400);
		tbRuleString->setColor({ 50,50,50,255 });
		tbRuleString->radius = 3;
		pnlSettings->add(tbRuleString);


		//TODO radio_boundaryHandlingMethod;
	}
};

int main(int argc, char* args[])
{
	MyApp app;
	app.begin();

	return 0;
}