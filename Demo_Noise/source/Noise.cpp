#include "Noise.h"

bool Noise::onInit()
{
	// load assets
	if (!assetManagerWrapper->loadResources())
	{
		std::cout << "Failed to load resources! See console output. Aborting." << std::endl;
		return false;
	}

	// set up window
	ggWindow.setTitle("Noise Demo");
	ggWindow.setResizable(true);
	ggWindow.setSize(1280, 720);
	ggWindow.centerWindowPosition();

	// tell canvas to use loaded font
	canvas.setFont(assetManagerWrapper->getFont());

	// subscribe to mouse and keyboard events
	inputManager.addKeyEventListener(this);
	inputManager.addMouseEventListener(this);

	// init UI
	Text::init(canvas.getSDLRenderer(), assetManagerWrapper->getFont()); // TODO: make this a part of the engine?
	defineElements();

	return true;
}

void Noise::onQuit()
{
	root->destroySelfAndChildren();
	Text::close();
}

void Noise::onLoop()
{
	// update perlin viewer with user set parameters from UI
	perlinViewer->velX = sliderVelX->value;
	perlinViewer->velY = sliderVelY->value;
	perlinViewer->velZ = sliderVelZ->value;
	perlinViewer->octaves = sliderOctaves->value;
	perlinViewer->initialAmplitude = sliderInitialAmplitude->value;
	perlinViewer->initialFrequency = sliderInitialFrequency->value;
	perlinViewer->resolutionDivision = sliderResolutionDivision->value;
	perlinViewer->roundNoise = cbRoundNoise->isChecked;

	// update perlin noise position based on velocity
	perlinViewer->updateVelocities(deltaTime());

	// update perlin noise position labels
	lblPosX->setText(std::to_string(perlinViewer->posX));
	lblPosY->setText(std::to_string(perlinViewer->posY));
	lblPosZ->setText(std::to_string(perlinViewer->posZ));

	// update fps label
	frameTimesAcc += deltaTime();
	framesCount++;
	if (frameTimesAcc >= 1)
	{
		float avgSpf = frameTimesAcc / framesCount;
		avgFps = 1 / avgSpf;

		framesCount = 0;
		frameTimesAcc = 0.0f;
	}
	lblFPS->setText(std::to_string((int)avgFps));

	// get updated window resolution
	std::string windowResolutionText = std::to_string(ggWindow.getWidth()) + " x " + std::to_string(ggWindow.getHeight());
	lblWindowSize->setText(windowResolutionText);

	// get updated cursor position
	std::string cursorPosText = std::to_string(inputManager.getMouseX()) + ", " + std::to_string(inputManager.getMouseY());
	lblCursorPos->setText(cursorPosText);
	
	// clear screen
	canvas.setColor(0x2B, 0x5C, 0x32);
	canvas.clear();

	// position and render UI
	int padding = 0;
	root->setWidthAbs(ggWindow.getWidth() - 2*padding);
	root->setHeightAbs(ggWindow.getHeight() - 2*padding);
	root->setXAbs(padding);
	root->setYAbs(padding);
	root->calculateLayout();
	root->render(&canvas);

	// update screen
	canvas.present();
}

void Noise::onKeyEvent(KeyEventType keyEventType, SDL_Keycode key)
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
}

void Noise::onMouseEvent(MouseEventType mouseEventType, int mouseX, int mouseY)
{
}

void Noise::sizeButton(Button* button)
{
	int w, h;
	canvas.getTextDimensions(button->text, &w, &h);
	button->setWidthAbs(w + button->paddingLeft + button->paddingRight);
	button->setHeightAbs(h + button->paddingTop + button->paddingBottom);
}
Label* Noise::createLabel(std::string labelText)
{
	Label* label = new Label(labelText);
	label->setForeColor({ 255,255,255,255 });
	
	label->verticalAutosize = true;
	label->horizontalAutosize = true;
	
	return label;
}
Slider* Noise::createSlider(float min, float max, float interval, float startingVal)
{
	Slider* slider = new Slider;
	slider->setWidthAbs(150);
	slider->min = min;
	slider->max = max;
	slider->interval = interval;
	slider->value = startingVal;

	return slider;
}
Container* Noise::createPanel(std::string panelTitle, Button* btnReset, std::vector<Control*> controls, std::vector<std::string> controlLabels)
{
	Container* panel = new Container;
	panel->layoutDirection = LayoutDirection::TOP_TO_BOTTOM;
	panel->horizontalAutosize = false;
	panel->verticalAutosize = true;
	panel->setColor(0x407848);
	panel->setPadding(12,12,8,8);
	panel->setChildGap(10);
	panel->borderColor = { 38, 84, 44, 255 };
	panel->borderThickness = 4;
	panel->shadowThickness = 6;
	
	Container* titleBar = new Container;
	titleBar->layoutDirection = LayoutDirection::LEFT_TO_RIGHT;
	titleBar->verticalAlignmentMode = VAlignmentMode::CENTER;
	titleBar->isVisible = false;
	titleBar->verticalAutosize = true;
	
	titleBar->add(createLabel(panelTitle));
	if (btnReset != nullptr)
	{
		titleBar->add(new HorizontalSpacer(100));
		titleBar->add(btnReset);
	}

	panel->add(titleBar);

	HorizontalDivider* divider = new HorizontalDivider(1);
	divider->color = panel->borderColor;
	panel->add(divider);

	for (int i = 0; i < controls.size(); i++)
	{
		Container* bar = new Container;
		bar->layoutDirection = LayoutDirection::LEFT_TO_RIGHT;
		bar->verticalAlignmentMode = VAlignmentMode::CENTER;
		bar->isVisible = false;
		bar->verticalAutosize = true;

		bar->add(createLabel(controlLabels[i]));
		bar->add(new HorizontalSpacer(100));
		bar->add(controls[i]);

		panel->add(bar);
	}

	return panel;
}
Button* Noise::createResetButton()
{
	Button* btnReset = new Button;
	btnReset->setText("Reset");
	btnReset->setPadding(8, 8, 2, 2);
	btnReset->radius = 3;
	btnReset->backColor = { 255,255,255,96 };
	btnReset->backColorOnHover = { 255,255,255,128};
	btnReset->backColorOnMouseDown = { 255,255,255,196};
	btnReset->foreColor = { 0,0,0,196 };
	btnReset->borderThickness = 0;
	btnReset->shadowThickness = 5;
	sizeButton(btnReset);
	return btnReset;
}

void Noise::defineElements()
{
	Element::inputManager = &inputManager;

	root = new Container();
	root->setXAbs(0);
	root->setYAbs(0);
	root->setPadding(24);
	root->setChildGap(26);
	root->layoutDirection = LayoutDirection::LEFT_TO_RIGHT;
	root->setWidthAbs(1280);
	root->setHeightAbs(720);
	root->isVisible = false;

	Container* sidebar = new Container;
	sidebar->layoutDirection = LayoutDirection::TOP_TO_BOTTOM;
	sidebar->verticalAlignmentMode = VAlignmentMode::TOP;
	sidebar->horizontalAutosize = true;
	sidebar->setPadding(0);
	sidebar->setChildGap(26);
	sidebar->isVisible = false;
	// sidebar->setColor(0xff00ff);
	root->add(sidebar);

	perlinViewer = new PerlinViewer();
	root->add(perlinViewer);

	// create controls
	lblWindowSize = createLabel();
	lblCursorPos = createLabel();
	lblPosX = createLabel();
	lblPosY = createLabel();
	lblPosZ = createLabel();
	lblFPS = createLabel();
	sliderVelX = createSlider(-100, 100, 1, 0);
	sliderVelY = createSlider(-100, 100, 1, 0);
	sliderVelZ = createSlider(-100, 100, 1, 0);
	sliderOctaves = createSlider(1, 8, 1, 1);
	sliderInitialAmplitude = createSlider(0.1f, 2.0f, 0.1f, 1.4f);
	sliderInitialFrequency = createSlider(0.1f, 8.0f, 0.1f, 0.4f);
	sliderResolutionDivision = createSlider(16, 48, 4, 24);
	cbRoundNoise = new Toggle();
	btnResetPosition = createResetButton();
	btnResetPosition->setOnClick([&]() {
		// TODO: so apparently the memory address at which some object is located can change, but the pointer will not change to that updated location, so it will be an invalid pointer --> seriously reconsider how we represent the UI hierarchy in memory!
		perlinViewer->posX = 0;
		perlinViewer->posY = 0;
		perlinViewer->posZ = 0;
	});
	btnResetVelocity = createResetButton();
	btnResetVelocity->setOnClick([&]() {
		perlinViewer->velX = 0;
		perlinViewer->velY = 0;
		perlinViewer->velZ = 0;
		sliderVelX->value = 0;
		sliderVelY->value = 0;
		sliderVelZ->value = 0;
	});

	// create position panel
	Container* pnlPositions = createPanel("Positions", btnResetPosition, { lblPosX, lblPosY, lblPosZ}, { "X", "Y", "Z"});
	sidebar->add(pnlPositions);

	// create velocity panel
	Container* pnlVelocity = createPanel("Velocity", btnResetVelocity, { sliderVelX, sliderVelY, sliderVelZ}, { "X", "Y", "Z"});
	sidebar->add(pnlVelocity);

	// create rendering panel
	Container* pnlRendering = createPanel("Rendering", nullptr, { sliderResolutionDivision, cbRoundNoise, lblFPS}, { "Resolution division", "Round noise", "FPS"});
	sidebar->add(pnlRendering);

	// create FBM panel
	Container* pnlFBM = createPanel("Fractal Brownian Motion", nullptr, { sliderOctaves, sliderInitialFrequency, sliderInitialAmplitude}, { "Octaves", "Initial frequency", "Initial amplitude"});
	sidebar->add(pnlFBM);

	// create UI debug panel 
	Container* pnlUI = createPanel("UI", nullptr, { lblWindowSize, lblCursorPos }, { "Window size", "Cursor position"});
	sidebar->add(pnlUI);


	// TODO
	//root->add(new Scrollable);
}
