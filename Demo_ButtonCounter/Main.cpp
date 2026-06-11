#include "GG/Core/Application.h"

class MyApp : public GG::Application
{
private:
	GG::RootContainer* root = nullptr;

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
		window.setResizable(false);
		window.centerWindowPosition();
		window.setSize(640, 480);

		// define root container
		root = new GG::RootContainer;
		root->layoutDirection = GG::LayoutDirection::TOP_TO_BOTTOM;
		root->setPadding(30);
		root->setChildGap(30);
		root->setInputManager(&inputManager);

		// create label
		GG::Label* label = new GG::Label("0");
		label->setHAlignment(GG::HAlignmentMode::CENTER); // center align label text horizontally
		root->add(label);

		// create button
		GG::Button* button = new GG::Button("Click me!");
		button->setOnClick([=]() {
			int numClicks = std::stoi(label->getText());
			label->setText(std::to_string(numClicks + 1));
		});
		button->setMaxHeight(80);
		button->setMaxWidth(300);
		root->add(button);
	}
};

int main(int argc, char* args[])
{
	MyApp app;
	app.begin();

	return 0;
}
