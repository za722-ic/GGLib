#include "GG/Core/Application.h"

class MyApp : public GG::Application
{
private:
	GG::RootContainer* root = nullptr;

	std::vector<GG::Container*> panels;
	std::vector<GG::Button*> sidebarButtons;

private:
	bool onInit() override
	{
		// load font
		assetManager.setBaseAssetPath("resources/");
		assetManager.loadFont("NotoSans-Medium.ttf", 20);

		// init text subsystems
		canvas.setFont(assetManager.getFont("NotoSans-Medium.ttf").value());
		GG::Text::init(canvas.getSDLRenderer(), assetManager.getFont("NotoSans-Medium.ttf").value());

		// define UI
		defineUI();

		// tell library set up was successful
		return true;
	}

	void onQuit() override
	{
		// close text subsystem
		GG::Text::close();

		// release the UI
		root->destroySelfAndChildren();
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
	void defineUI()
	{
		// set up window
		window.setTitle("MyApp");
		window.setResizable(true);
		window.centerWindowPosition();
		window.setSize(1280, 720);

		// define root container
		root = new GG::RootContainer;
		root->layoutDirection = GG::LayoutDirection::LEFT_TO_RIGHT;
		root->setPadding(30);
		root->setChildGap(30);
		root->setInputManager(&inputManager);

		// define sidebar
		GG::Container* sidebar = new GG::Container;
		sidebar->layoutDirection = GG::LayoutDirection::TOP_TO_BOTTOM;
		sidebar->verticalAlignmentMode = GG::VAlignmentMode::TOP;
		sidebar->horizontalAlignmentMode = GG::HAlignmentMode::CENTER;
		sidebar->horizontalAutosize = true;
		sidebar->setPadding(5);
		sidebar->setChildGap(5);
		sidebar->setColor(30);
		root->add(sidebar);

		GG::Container* pnlContentRoot = new GG::Container;
		root->add(pnlContentRoot);

		// define sidebar buttons and their corresponing panels
		for (int i = 0; i < 8; i++)
		{
			GG::Container* pnlContent = new GG::Container;
			pnlContent->setColor(i * 20);
			panels.push_back(pnlContent);


			GG::Button* btnSidebarSelector = new GG::Button(std::to_string(i));
			btnSidebarSelector->setPadding(5);
			btnSidebarSelector->setHeightAbs(60);
			btnSidebarSelector->setWidthAbs(60);
			btnSidebarSelector->borderThickness = 1;
			btnSidebarSelector->setOnClick([=]() {
				pnlContentRoot->removeAllChildren();
				pnlContentRoot->add(pnlContent);

				for (auto btn : sidebarButtons)
				{
					btn->borderColor = { 255,255,255,255 };
				}
				btnSidebarSelector->borderColor = { 255, 127, 0, 255 };
			});
			sidebarButtons.push_back(btnSidebarSelector);
			sidebar->add(btnSidebarSelector);
		}

		definePanel0();
	}

	void definePanel0()
	{
		GG::Container* panel = panels.at(0);
		panel->layoutDirection = GG::LayoutDirection::TOP_TO_BOTTOM;
		panel->verticalAlignmentMode = GG::VAlignmentMode::TOP;

		GG::Container* topPanel = new GG::Container;
		topPanel->setColor(255, 0, 255);
		topPanel->layoutDirection = GG::LayoutDirection::LEFT_TO_RIGHT;
		topPanel->verticalAutosize = true;
		topPanel->setChildGap(3);
		topPanel->horizontalAlignmentMode = GG::HAlignmentMode::LEFT;
		panel->add(topPanel);

		for (int i = 0; i < 4; i++)
		{
			GG::Button* btnTopPanel = new GG::Button("Option" + std::to_string(i));
			btnTopPanel->setWidthAbs(90);
			btnTopPanel->setHeightAbs(30);
			
			topPanel->add(btnTopPanel);
		}

		for (int r = 0; r < 3; r++)
		{
			GG::Container* rowContainer = new GG::Container;
			rowContainer->isVisible = false;
			rowContainer->layoutDirection = GG::LayoutDirection::LEFT_TO_RIGHT;
			rowContainer->verticalAutosize = true;
			rowContainer->horizontalAlignmentMode = GG::HAlignmentMode::CENTER;
			rowContainer->setPadding(10);
			rowContainer->setChildGap(10);
			panel->add(rowContainer);

			for (int c = 0; c < 3; c++)
			{
				GG::Container* fauxPanel = new GG::Container;
				fauxPanel->setWidthAbs(300);
				fauxPanel->setHeightAbs(200);
				fauxPanel->setColor({ 30,30,30,255 });
				fauxPanel->radius = 3;
				rowContainer->add(fauxPanel);
			}
		}

		
	}
};

int main(int argc, char* args[])
{
	MyApp app;
	app.begin();

	return 0;
}
