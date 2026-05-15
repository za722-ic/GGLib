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
		ggWindow.setTitle("FPS: " + std::to_string(avgFps) + " | " + std::to_string(oldRenderer));

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

	if (key == SDLK_SPACE)
	{
		oldRenderer = !oldRenderer;
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
	root->setColor(0x222222ff);
	root->radius = 0;
	root->layoutMode = LayoutMode::FLEX;
	root->horizontalAlignmentMode = HAlignmentMode::CENTER;
	root->verticalAlignmentMode = VAlignmentMode::CENTER;
	root->layoutDirection = LayoutDirection::LEFT_TO_RIGHT;
	root->setPadding(60);
	root->setChildGap(60);

	// create button
	Button* button = new Button;
	button->setText("Hello");
	// button->horizontalAutosize = true;
	// button->verticalAutosize = true;
	button->setOnClick([=]()
	{
		SDL_Color newColor = { (unsigned char)MoreMath::random(128, 255), (unsigned char)MoreMath::random(128, 255), (unsigned char)MoreMath::random(128, 255), 255 };
		SDL_Color newColorOnHover = { 0.8f * newColor.r, 0.8f * newColor.g, 0.8f * newColor.b, newColor.a };
		SDL_Color newColorOnMouseDown= { 0.6f * newColor.r, 0.6f * newColor.g, 0.6f * newColor.b, newColor.a };
		button->backColor = newColor;
		button->backColorOnHover = newColorOnHover;
		button->backColorOnMouseDown = newColorOnMouseDown;
	});
	// root->add(button);

	Label* label = new Label(loremIpsum);
	label->setColor(255,255,255,20);
	label->setTextPadding(20);
	label->setAlignment(TTF_HORIZONTAL_ALIGN_CENTER);
	label->setAutoSizeHorizontal(true);
	label->setAutoSizeVertical(true);
	root->add(label);
}
