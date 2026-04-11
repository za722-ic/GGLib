#include "Noise.h"

bool Noise::onInit()
{
	// load assets
	if (!noiseAssetManager->loadResources())
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
	canvas.setFont(noiseAssetManager->getFont());

	// subscribe to mouse and keyboard events
	inputManager.addKeyEventListener(this);
	inputManager.addMouseEventListener(this);

	// init UI
	defineElements();

	return true;
}

void Noise::onQuit()
{
	root->destroySelfAndChildren();
}

int oldsidebarwidth;
void Noise::onLoop()
{
	// clear screen
	canvas.setColor(0x2B, 0x5C, 0x32);
	canvas.clear();

	// update and draw fps
	frameTimesAcc += deltaTime();
	framesCount++;
	if (frameTimesAcc >= 1)
	{
		float avgSpf = frameTimesAcc / framesCount;
		avgFps = 1 / avgSpf;

		framesCount = 0;
		frameTimesAcc = 0.0f;
	}
	lblFPS->text = "FPS: " + std::to_string(avgFps);

	// get updated window resolution
	std::string windowResolutionText = "Window size: " + std::to_string(ggWindow.getWidth()) + "x" + std::to_string(ggWindow.getHeight());
	lblWindowSize->setText(windowResolutionText);

	// get updated cursor position
	std::string cursorPosText = "Cursor position: " + std::to_string(inputManager.getMouseX()) + ", " + std::to_string(inputManager.getMouseY());
	lblCursorPos->setText(cursorPosText);

	int padding = 0;
	root->setWidthAbs(ggWindow.getWidth() - 2*padding);
	root->setHeightAbs(ggWindow.getHeight() - 2*padding);
	root->setX(padding);
	root->setY(padding);
	root->calculateLayout();
	//root->render(&canvas);

	canvas.setColor(255, 255, 255, 135);
	int x = 100;
	int y = 100;
	int w = 400;
	int h = 400;
	int r = 50;
	int thickness = 20;
	int numTriangles = 12;
	int outerRadius = r + thickness;
	canvas.drawRoundedRect(x-thickness, y-thickness, w+2*thickness, h+2*thickness, outerRadius, thickness, numTriangles);
	canvas.fillRoundedRect(x, y, w, h, r, numTriangles);

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

void Noise::defineElements()
{
	root = new Container();
	root->setX(0);
	root->setY(0);
	root->setColor(255,0,255,0);
	root->setPadding(12);
	root->setChildGap(12);
	root->layoutDirection = LayoutDirection::LEFT_TO_RIGHT;
	root->setWidthAbs(1280);
	root->setHeightAbs(720);

	sidebar = new Container;
	sidebar->layoutDirection = LayoutDirection::TOP_TO_BOTTOM;
	sidebar->verticalAlignmentMode = VAlignmentMode::TOP;
	sidebar->horizontalAutosize = true;
	sidebar->setPadding(0);
	sidebar->setChildGap(5);
	sidebar->setColor(255, 0, 255, 0);
	root->add(sidebar);

	Control* perlinViewer = new Control;
	perlinViewer->setColor(0x41C7FF);
	root->add(perlinViewer);




	Container*  pnlPositions = new Container;
	pnlPositions->layoutDirection = LayoutDirection::TOP_TO_BOTTOM;
	pnlPositions->verticalAlignmentMode = VAlignmentMode::CENTER;
	pnlPositions->horizontalAutosize = true;
	pnlPositions->verticalAutosize = true;
	pnlPositions->setColor(0x407848);
	pnlPositions->setPadding(12,12,8,8);
	pnlPositions->setChildGap(0);
	sidebar->add(pnlPositions);

	Container* titlebar = new Container;
	titlebar->verticalAutosize = true;
	pnlPositions->add(titlebar);

	Label *lblPositionsTitle = new Label();
	lblPositionsTitle->text = "Positions";
	lblPositionsTitle->foreColor = { 255, 255, 255 };
	lblPositionsTitle->setWidthAbs(200);
	lblPositionsTitle->setHeightAbs(25);
	titlebar->add(lblPositionsTitle);

	// FPS label
	lblFPS = new Label();
	lblFPS->text = "FPS: ???";
	lblFPS->foreColor = { 255, 255, 255 };
	lblFPS->setWidthAbs(200);
	lblFPS->setHeightAbs(25); // TODO: if i omit this, things get weird --> add some default dimensions the Element class via the construvtor
	pnlPositions->add(lblFPS);
	
	// resolution label
	lblWindowSize = new Label();
	lblWindowSize->text = "Window size: ???";
	lblWindowSize->foreColor = { 255, 255, 255 };
	lblWindowSize->setWidthAbs(300);
	lblWindowSize->setHeightAbs(25);
	pnlPositions->add(lblWindowSize);
	
	// mouse cursor position label
	lblCursorPos = new Label();
	lblCursorPos->text = "Cursor position: ";
	lblCursorPos->foreColor = { 255, 255, 255 };
	lblCursorPos->setWidthAbs(200);
	lblCursorPos->setHeightAbs(25);
	pnlPositions->add(lblCursorPos);


}
