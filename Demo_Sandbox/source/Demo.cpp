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

Label* Demo::createLabel(std::string labelText)
{
	Label* label = new Label(labelText);
	label->setForeColor({ 255,255,255,255 });


	label->labAutosize = true;

	return label;
}
Container* Demo::createPanel(std::string panelTitle, Button* btnReset, std::vector<Control*> controls, std::vector<std::string> controlLabels)
{
	Container* panel = new Container;
	panel->layoutDirection = LayoutDirection::TOP_TO_BOTTOM;
	panel->horizontalAutosize = false;
	panel->verticalAutosize = true;
	panel->setColor(0x407848);
	panel->setPadding(64, 64, 64, 64);
	panel->setChildGap(10);
	panel->borderColor = { 38, 84, 44, 255 };
	panel->borderThickness = 4;
	panel->shadowThickness = 6;

	Container* titleBar = new Container;
	titleBar->layoutDirection = LayoutDirection::LEFT_TO_RIGHT;
	titleBar->verticalAlignmentMode = VAlignmentMode::CENTER;
	//titleBar->isVisible = false;
	titleBar->setColor(255, 255, 0, 255);
	titleBar->verticalAutosize = true;

	titleBar->add(createLabel(panelTitle));
	if (btnReset != nullptr)
	{
		titleBar->add(new HorizontalSpacer(100));
		titleBar->add(btnReset);
	}

	panel->add(titleBar);


//	return panel;


	
	HorizontalDivider* divider = new HorizontalDivider(1);
	divider->color = panel->borderColor;
	panel->add(divider);

	for (int i = 0; i < controls.size(); i++)
	{
		Container* bar = new Container;
		bar->layoutDirection = LayoutDirection::LEFT_TO_RIGHT;
		bar->verticalAlignmentMode = VAlignmentMode::CENTER;
		bar->verticalAutosize = true;
		///bar->isVisible = false;
		bar->setColor(255, 0, 255, 255);

		bar->add(createLabel(controlLabels[i]));
		bar->add(new HorizontalSpacer(100));
		bar->add(controls[i]);

		panel->add(bar);
	}

	return panel;
}

void Demo::defineElements()
{
	// TODO: automate this
	Element::inputManager = &inputManager;

	// create root container
	root = new Container;
	root->setColor(200,200,200,255);
	root->radius = 0;
	root->layoutMode = LayoutMode::FLEX;
	root->horizontalAlignmentMode = HAlignmentMode::CENTER;
	root->verticalAlignmentMode = VAlignmentMode::CENTER;
	root->layoutDirection = LayoutDirection::LEFT_TO_RIGHT;
	root->setPadding(60);
	root->setChildGap(60);

	auto sliderOctaves = createSlider(1, 8, 1, 1);
	auto sliderInitialAmplitude = createSlider(0.1f, 2.0f, 0.1f, 1.4f);
	auto sliderInitialFrequency = createSlider(0.1f, 8.0f, 0.1f, 0.4f);

	
	// create FBM panel
	Container* pnlFBM = createPanel("Fractal Brownian Motion", nullptr, { sliderOctaves, sliderInitialFrequency, sliderInitialAmplitude }, { "Octaves", "Initial frequency", "Initial amplitude" });
	//Container* pnlFBM = createPanel("Fractal Brownian Motion", nullptr, {  }, { });
	root->add(pnlFBM);

	//Container* alpha = new Container;
	//alpha->layoutMode = LayoutMode::FLEX;
	//alpha->setPadding(10);
	//alpha->horizontalAutosize = false;
	//alpha->verticalAutosize = false;
	//alpha->setColor(40, 40, 40, 255);
	////alpha->setMaxWidth(100);
	//root->add(alpha);

	//
	//Label* label = new Label(loremIpsum);
	//label->setColor(200,0,230);
	//label->setPadding(20);
	//label->setAlignment(TTF_HORIZONTAL_ALIGN_CENTER);
	//label->horizontalAutosize = true;
	//label->verticalAutosize = true;
	//label->setMaxWidth(300);
	//alpha->add(label);
}
