#include "Demo.h"

/*
TODO: LAG

Experiment: render 400 rects using SDL_RenderFillRect
			then render 400 rects using the canvas

			(*perform it pls)

Hypothesis: the latter is way slower

Course of action if true: we need a much more optimised sequence of draw calls to the graphics lib.
						  general outline:

				  +--------------------+                                      +--------------+
				  | DESCRIPTION OF GUI | ----> (Sequence of draw calls) ----> | GRAPHICS LIB | 
				  +--------------------+                                      +--------------+

Aight so experiment showed it's actually text rendering that's slow, so focus on that instead --> cache rendering results in controls
*/


bool Demo::onInit()
{
	// load assets
	if (!demoAssetManager->loadResources())
	{
		std::cout << "Failed to load resources! See console output. Aborting." << std::endl;
		return false;
	}

	// set up window
	window.setTitle("Demo");
	window.setResizable(true);
	window.setSize(1280, 720);
	window.centerWindowPosition();

	// tell canvas to use loaded font
	canvas.setFont(demoAssetManager->getFont());

	// subscribe to mouse and keyboard events
	inputManager.addKeyEventListener(this);
	inputManager.addMouseEventListener(this);

	// init UI
	defineElements();

	return true;
}

void Demo::onQuit()
{
	root->destroySelfAndChildren();
}

void Demo::onLoop()
{
	// clear screen
	canvas.setColor(0x58, 0x5B, 0x65);
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
	std::string windowResolutionText = "Window size: " + std::to_string(window.getWidth()) + "x" + std::to_string(window.getHeight());
	lblWindowSize->setText(windowResolutionText);

	// get updated cursor position
	std::string cursorPosText = "Cursor position: " + std::to_string(inputManager.getMouseX()) + ", " + std::to_string(inputManager.getMouseY());
	lblCursorPos->setText(cursorPosText);

	int padding = 10;
	root->setWidthAbs(window.getWidth() - 2*padding);
	root->setHeightAbs(window.getHeight() - 2*padding);
	root->setX(padding);
	root->setY(padding);
	root->calculateLayout();
	root->render(&canvas);

	// update screen
	canvas.present();
}

void Demo::onKeyEvent(GG::KeyEventType keyEventType, SDL_Keycode key)
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
		window.toggleFullScreen();
	}
}

void Demo::onMouseEvent(GG::MouseEventType mouseEventType, int mouseX, int mouseY)
{
}

void Demo::defineElements()
{
	root = new GG::Container();
	root->setX(0);
	root->setY(0);
	root->setColor(0x292929);
	root->setPadding(10);
	root->setChildGap(2);
	root->layoutDirection = LayoutDirection::TOP_TO_BOTTOM;
	root->setWidthAbs(1280);
	root->setHeightAbs(720);

	Container *topbar = new Container;
	topbar->layoutDirection = LayoutDirection::LEFT_TO_RIGHT;
	topbar->setHeightAbs(64);
	topbar->setColor(0x030303);
	topbar->setPadding(12);
	topbar->setChildGap(12);
	topbar->horizontalAlignmentMode = HAlignmentMode::RIGHT;
	root->add(topbar);

	Container *body = new Container;
	body->layoutDirection = LayoutDirection::LEFT_TO_RIGHT;
	body->setPadding(20);
	body->setChildGap(20);
	body->setColor(0xf92929);
	root->add(body);

	Container *bottombar = new Container;
	bottombar->layoutDirection = LayoutDirection::LEFT_TO_RIGHT;
	bottombar->setHeightAbs(64);
	bottombar->setColor(0x030303);
	bottombar->verticalAlignmentMode = VAlignmentMode::TOP;
	root->add(bottombar);


	Container *sidebar = new Container;
	sidebar->layoutDirection = LayoutDirection::TOP_TO_BOTTOM;
	sidebar->verticalAlignmentMode = VAlignmentMode::BOTTOM;
	sidebar->setWidthAbs(64);
	sidebar->setPadding(12);
	sidebar->setChildGap(12);
	sidebar->setColor(0x030303);
	body->add(sidebar);

	Container* bodybody = new Container;
	bodybody->layoutDirection = LayoutDirection::LEFT_TO_RIGHT;
	bodybody->verticalAlignmentMode = VAlignmentMode::CENTER;
	bodybody->setChildGap(12);
	bodybody->setPadding(12);
	bodybody->setColor(0xc994f7);
	body->add(bodybody);


	for (size_t i = 0; i < 3; i++)
	{
		Button *sidebarChild = new Button;
		sidebarChild->setHeightAbs(32);
		sidebarChild->setColor(0x392929);
		sidebar->add(sidebarChild);
	}

	Control *bbChild1 = new Control;
	bbChild1->setPreferredWidth(100);
	bbChild1->setMinWidth(10);
	bbChild1->setMaxWidth(100);
	bbChild1->setMaxHeight(100);
	bbChild1->setColor(0x191919);
	bodybody->add(bbChild1);

	Control *bbChild2 = new Control;
	bbChild2->setColor(0x19f919);
	bbChild2->setMinHeight(300);
	bbChild2->setMinWidth(10);
	bodybody->add(bbChild2);
	
	Container *bbChild3 = new Container;
	bbChild3->horizontalAutosize = true;
	bbChild3->verticalAutosize = true;
	bbChild3->setPadding(30);
	bbChild3->setPreferredWidth(100);
	bbChild3->setColor(0x191919);
	bodybody->add(bbChild3);
	
	Container *bbChild3Child = new Container;
	bbChild3Child->setWidthAbs(100);
	bbChild3Child->setHeightAbs(100);
	bbChild3Child->setColor(0x491919);
	bbChild3->add(bbChild3Child);

	// FPS label
	lblFPS = new Label();
	lblFPS->text = "FPS: ???";
	lblFPS->foreColor = { 255, 255, 255 };
	lblFPS->setWidthAbs(200);
	bottombar->add(lblFPS);
	
	// resolution label
	lblWindowSize = new Label();
	lblWindowSize->text = "Window size: ???";
	lblWindowSize->foreColor = { 255, 255, 255 };
	lblWindowSize->setWidthAbs(300);
	bottombar->add(lblWindowSize);
	
	// mouse cursor position label
	lblCursorPos = new Label();
	lblCursorPos->text = "Cursor position: ";
	lblCursorPos->foreColor = { 255, 255, 255 };
	lblCursorPos->setWidthAbs(200);
	bottombar->add(lblCursorPos);

	for (size_t i = 0; i < 3; i++)
	{
		Button *topbarChild = new Button;
		topbarChild->setWidthAbs(32);
		topbarChild->setBackColor(0x39, 0x29, 0x29);
		topbarChild->setOnMouseEnter([=](){	topbarChild->setBackColor(0x29, 0x29, 0x29); });
		topbarChild->setOnMouseExit([=](){	topbarChild->setBackColor(0x39, 0x29, 0x29); });
		topbarChild->setOnClick([=]() {	std::cout << "HEJ FROM topbarChild" << i << std::endl; });
		inputManager.addMouseEventListener(topbarChild);
		topbar->add(topbarChild);
	}

	/*
		TODO: if I add the same element twice, then it obvoiusly breaks because of double deletion(and other stuff)

		someElement->add(child4);
		someOtherElement->add(child4);
		
		but maybe making something like this easier is a worthy consideration for the API
	*/
}
