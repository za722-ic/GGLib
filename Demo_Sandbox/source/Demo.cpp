#include "Demo.h"

bool Demo::onInit()
{
	// load assets
	if (!demoAssetManager->loadResources())
	{
		std::cout << "Failed to load resources! See console output. Aborting." << std::endl;
		return false;
	}

	// set up window
	ggWindow.setTitle("Demo");
	ggWindow.setResizable(true);
	ggWindow.setSize(1280, 720);
	ggWindow.centerWindowPosition();

	// tell canvas to use loaded font
	canvas.setFont(demoAssetManager->getFont());

	// subscribe to mouse and keyboard events
	inputManager.addKeyEventListener(this);
	inputManager.addMouseEventListener(this);

	// init UI
	Text::init(canvas.getSDLRenderer(), demoAssetManager->getFont());
	defineElements();


	return true;
}

void Demo::onQuit()
{
	Text::close();
}

void Demo::onLoop()
{
	frameTimesAcc += deltaTime();
	framesCount++;
	if (frameTimesAcc >= 1.0f)
	{
		avgFps = framesCount;
		ggWindow.setTitle("FPS: " + std::to_string(avgFps));

		frameTimesAcc = 0.0f;
		framesCount = 0;
	}

	// clear screen
	canvas.setColor(0x58, 0x5B, 0x65);
	canvas.clear();

	// draw UI
	root->setWidthAbs(ggWindow.getWidth());
	root->setHeightAbs(ggWindow.getHeight());
	root->calculateLayout();
	root->render(&canvas);

	// update screen
	canvas.present();
}

void Demo::onKeyEvent(KeyEventType keyEventType, SDL_Keycode key)
{
	// The following events are all for when the key is pressed down, not when released
	if (keyEventType == KEY_UP) return;

	// Exit on ESC key pressed
	if (key == SDLK_ESCAPE)
	{
		isRunning = false;
		return;
	}

	if (key == SDLK_F11)
	{
		ggWindow.toggleFullScreen();
	}

	auto ptToPx = [](float pt) { return pt * 1.3333333f; };
	auto pxToPt = [](float px) { return px / 1.3333333f; };
	if (key == SDLK_EQUALS || key == SDLK_MINUS)
	{
		int incrementMag = 5;

		int increment;
		if (key == SDLK_EQUALS)
			increment = +incrementMag;
		else
			increment = -incrementMag;

		TTF_Font* font = demoAssetManager->getFont();
		float fontSize_Pt = TTF_GetFontSize(font);
		float fontSize_Px = ptToPx(fontSize_Pt);
		float newFontSize_Px = fontSize_Px + increment;
		float newFontSize_Pt = pxToPt(newFontSize_Px);
		TTF_SetFontSize(font, newFontSize_Pt);
	}
}

void Demo::onMouseEvent(MouseEventType mouseEventType, int mouseX, int mouseY)
{
}

void Demo::defineElements()
{
	// TODO: automate this
	Element::inputManager = &inputManager;

	// create root container
	root = new Container;
	root->setColor(210,210,180,255);
	root->radius = 0;
	root->layoutMode = LayoutMode::FLEX;
	root->horizontalAlignmentMode = HAlignmentMode::CENTER;
	root->verticalAlignmentMode = VAlignmentMode::CENTER;
	root->layoutDirection = LayoutDirection::LEFT_TO_RIGHT;
	root->setPadding(60);
	root->setChildGap(60);

	std::vector<unsigned int> colors =
	{
		0xcc550000,
		0xcc2c5f34,
		0xcc000080
	};
	std::vector<SDL_Color> borderColors =
	{
		{84,44,37,255},
		{37,84,44,255},
		{37,44,84,255}
	};
	for (int i = 0; i < 3; i++)
	{
		Container* panel = new Container;
		panel->layoutDirection = LayoutDirection::TOP_TO_BOTTOM;
		panel->horizontalAutosize = false;
		panel->verticalAutosize = true;
		panel->setColor(colors.at(i));
		panel->setPadding(32);
		panel->setChildGap(10);
		panel->borderColor = borderColors.at(i);
		panel->borderThickness = 4;
		panel->shadowThickness = 6;
		root->add(panel);

		Label* label = new Label(loremIpsum);
		label->verticalAutosize = true;
		label->setHAlignment(HAlignmentMode::CENTER);
		label->setVAlignment(VAlignmentMode::CENTER);
		label->setForeColor({ 255,255,255,165 });
		panel->add(label);
	}
}
