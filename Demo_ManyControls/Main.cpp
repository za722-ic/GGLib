#include "GG/Core/Application.h"

class MyApp : public GG::Application
{
private:
	GG::RootContainer* root = nullptr;

private:
	bool onInit() override
	{
		// set up window
		window.setTitle("MyApp");
		window.setResizable(true);
		window.setSize(300, 300);

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
		// have to set root container's position manually
		root = new GG::RootContainer;
		root->setInputManager(&inputManager);
		root->layoutMode = GG::LayoutMode::ABSOLUTE;
		
		// create button and add it to the UI
		int i = 0;
		const int size = 30;
		for (int x = 0; x < 1920 - size; x += size + 2)
		{
			for (int y = 0; y < 1008 - size; y += size + 2)
			{
				GG::Label* label = new GG::Label(std::to_string(i++));

				label->setWidthAbs(size);
				label->setHeightAbs(size);
				label->x = x;
				label->y = y;
				label->radius = 5;
				label->setColor({ 255,255,255,255 });
				label->setForeColor({ 0,0,0,255 });
				label->setHAlignment(GG::HAlignmentMode::CENTER);

				root->add(label);
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