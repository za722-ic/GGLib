#include "GG/Core/Application.h"

#include "GoLControl.h"

class MyApp : public GG::Application
{
private:
	GG::RootContainer* root = nullptr;
	GG::Container* pnlDropDownOptions = nullptr;
	GG::Combobox* cbBoundaryType;
	GoLControl* golControl = nullptr;
	GG::Slider* sliderSimulationSpeed = nullptr;
	GG::Toggle* togglePaused = nullptr;
	GG::Textbox* tbRuleString = nullptr;

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

		// update golControl
		golControl->update(deltaTime());
		golControl->stepDuration = sliderSimulationSpeed->value;
		golControl->isPaused = togglePaused->isChecked;
		golControl->boundaryType = cbBoundaryType->getSelectedOption() == "Toroidal" ? GoLControl::BoundaryType::TOROIDAL : GoLControl::BoundaryType::DEAD_EDGE;
		golControl->setRuleString(tbRuleString->getText());

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

		// create settings panel
		GG::Container* pnlSettings = new GG::Container;
		pnlSettings->layoutDirection = GG::LayoutDirection::LEFT_TO_RIGHT;
		pnlSettings->setChildGap(25);
		pnlSettings->setPadding(25);
		pnlSettings->setColor({ 30,30,30,255 });
		pnlSettings->verticalAutosize = true;
		pnlSettings->shadowThickness = 8;
		pnlSettings->radius = 0;
		root->add(pnlSettings);

		// create game of life custom control
		golControl = new GoLControl(&canvas);
		root->add(golControl);

		// create controls for settings
		sliderSimulationSpeed = new GG::Slider;
		sliderSimulationSpeed->setMaxWidth(200);
		sliderSimulationSpeed->isRenderValueStr = false;
		sliderSimulationSpeed->min = 0.05f;
		sliderSimulationSpeed->max = 2.0f;
		sliderSimulationSpeed->interval = 0.05f;
		sliderSimulationSpeed->value = 1.0f;
		pnlSettings->add(sliderSimulationSpeed);

		{
			GG::Container *pnlPause = new GG::Container;
			pnlPause->layoutDirection = GG::LayoutDirection::LEFT_TO_RIGHT;
			pnlPause->setChildGap(0);
			pnlPause->setPadding(8);
			pnlPause->horizontalAutosize = true;
			pnlPause->verticalAutosize = false;
			pnlSettings->add(pnlPause);

			GG::Label* lblPause = new GG::Label("Pause");
			pnlPause->add(lblPause);

			togglePaused = new GG::Toggle;
			pnlPause->add(togglePaused);
		}

		GG::Button* btnReturnToOrigin = new GG::Button("Return to origin");
		btnReturnToOrigin->setHeightAbs(30);
		btnReturnToOrigin->setWidthAbs(200);
		btnReturnToOrigin->radius = 3;
		btnReturnToOrigin->setOnClick([=]() {
			golControl->returnToOrigin();
			});
		pnlSettings->add(btnReturnToOrigin);

		GG::Button* btnClearCells = new GG::Button("Clear cells");
		btnClearCells->setHeightAbs(30);
		btnClearCells->setWidthAbs(200);
		btnClearCells->radius = 3;
		btnClearCells->setOnClick([=]() {
			golControl->clearCells();
		});
		pnlSettings->add(btnClearCells);

		tbRuleString = new GG::Textbox;
		tbRuleString->setText("B3/S23");
		tbRuleString->setHeightAbs(30);
		tbRuleString->setWidthAbs(400);
		tbRuleString->setColor({ 50,50,50,255 });
		tbRuleString->radius = 3;
		pnlSettings->add(tbRuleString);

		cbBoundaryType = new GG::Combobox("Select boundary type", root);
		cbBoundaryType->radius = 3;
		cbBoundaryType->setWidthAbs(200);
		cbBoundaryType->clearOptions();
		cbBoundaryType->addOption("Toroidal");
		cbBoundaryType->addOption("Dead Edge");
		cbBoundaryType->setSelectedOptionIndex(0);
		pnlSettings->add(cbBoundaryType);
	}
};

int main(int argc, char* args[])
{
	MyApp app;
	app.begin();

	return 0;
}