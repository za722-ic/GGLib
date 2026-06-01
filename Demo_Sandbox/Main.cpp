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
		assetManager.loadFont("NotoSans-Medium.ttf", 32);

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
		root->layoutMode = GG::LayoutMode::FLEX;
		root->setChildGap(0);
		root->setPadding(0);
		

		GG::Container* sc = new GG::Container;
		sc->verticalAlignmentMode = GG::VAlignmentMode::TOP;
		sc->setColor({ 255,0,180, 255 });
		sc->setScrollable(true);
		sc->setChildGap(200);
		sc->setPadding(200);
		root->add(sc);



		// slider
		GG::Container* sc2 = new GG::Container;
		sc2->verticalAlignmentMode = GG::VAlignmentMode::TOP;
		sc2->setScrollable(true);
		sc2->setColor({ 180,0,255,255 });
		sc2->setChildGap(200);
		sc2->setPadding(20);
		sc2->setMinHeight(900);
		sc->add(sc2);
		for (int i = 0; i < 30; i++)
		{
			GG::Slider* slider = new GG::Slider;
			slider->setWidthAbs(700);
			slider->min = 0;
			slider->max = 100;
			slider->interval = 1;
			slider->value = 50;
			sc2->add(slider);
		}

		// textboix
		GG::Container* sc3 = new GG::Container;
		sc3->verticalAlignmentMode = GG::VAlignmentMode::TOP;
		sc3->setColor({ 180,0,255,255 });
		sc3->setScrollable(true);
		sc3->setChildGap(200);
		sc3->setPadding(20);
		sc3->setMinHeight(900);
		sc->add(sc3);
		for (int i = 0; i < 30; i++)
		{
			GG::Textbox *textbox = new GG::Textbox;
			textbox->setWidthAbs(500);
			textbox->setHeightAbs(30);
			sc3->add(textbox);
		}

		// checkbox
		GG::Container* sc4 = new GG::Container;
		sc4->verticalAlignmentMode = GG::VAlignmentMode::TOP;
		sc4->setColor({ 180,0,255,255 });
		sc4->setChildGap(200);
		sc4->setScrollable(true);
		sc4->setPadding(20);
		sc4->setMinHeight(900);
		sc->add(sc4);
		for (int i = 0; i < 30; i++)
		{
			GG::Checkbox *checkbox= new GG::Checkbox;
			sc4->add(checkbox);
		}

		// toggle
		GG::Container* sc5 = new GG::Container;
		sc5->setColor({ 180,0,255,255 });
		sc5->setChildGap(200);
		sc5->verticalAlignmentMode = GG::VAlignmentMode::TOP;
		sc5->setScrollable(true);
		sc5->setPadding(20);
		sc5->setMinHeight(900);
		sc->add(sc5);
		for (int i = 0; i < 30; i++)
		{
			GG::Toggle *toggle = new GG::Toggle;
			sc5->add(toggle);
		}

		// button 
		GG::Container* sc6 = new GG::Container;
		sc6->setColor({ 180,0,255,255 });
		sc6->verticalAlignmentMode = GG::VAlignmentMode::TOP;
		sc6->setChildGap(200);
		sc6->setScrollable(true);
		sc6->setPadding(20);
		sc6->setMinHeight(900);
		sc->add(sc6);
		for (int i = 0; i < 30; i++)
		{
			GG::Button* button = new GG::Button("HELLO");
			button->setMinHeight(50);
			button->setOnClick([]() {std::cout << "Hello" << std::endl; });
			sc6->add(button);
		}

		// label
		GG::Container* sc7 = new GG::Container;
		sc7->setColor({ 180,0,255,255 });
		sc7->verticalAlignmentMode = GG::VAlignmentMode::TOP;
		sc7->setChildGap(200);
		sc7->setPadding(20);
		sc7->setScrollable(true);
		sc7->setMinHeight(900);
		sc->add(sc7);
		for (int i = 0; i < 30; i++)
		{
			GG::Label* label = new GG::Label(std::to_string(i) + " kaj asd asd as da sd asd as da sdasdasd asd asda sd asdasdasda sd asdoikaosdalksjd lk ajsd laksjd alksjd alksjd lkajsd alksjda lskdj alskjd sdklasj dasj dklasj dlkasj dklasj kdlja skldj aslkd jklas djklas jdklajs dklja slkdj alskjd lkasj dlkasj d");
			label->setForeColor({ 255,255,255,255 });
			sc7->add(label);
		}
	}
};

int main(int argc, char* args[])
{
	MyApp app;
	app.begin();

	return 0;
}