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
	ggWindow.setTitle("Demo");
	ggWindow.setResizable(true);
	ggWindow.setSize(1280, 720);

	// tell canvas to use loaded font
	canvas.setFont(demoAssetManager->getFont());

	// subscribe to mouse and keyboard events
	inputManager.addKeyEventListener(this);
	inputManager.addMouseEventListener(this);

	// init UI
	initUI();

	return true;
}

void Demo::onQuit()
{}

void Demo::onLoop()
{
	// clear screen
	canvas.setColor(30);
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


	// TODO: would be beneficial to abstract this away from the user. problem is you would still need to rely on the user clearing the screen and updating the screen --> how much should the user be able to control ui rendering? ideally as much as they wish, or as little as they wish...
	canvas.setColor(255); // TODO: this should not be a thing --> ui elements' color should be defined by ui elements
	ui.render();

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
}

void Demo::onMouseEvent(MouseEventType mouseEventType, int mouseX, int mouseY)
{
}

void Demo::initUI()
{
	ui.init(&canvas, &inputManager, &ggWindow);

	// (FPS) label
	lblFPS = new Label();
	lblFPS->x = 10; 
	lblFPS->y = 10;
	lblFPS->text = "FPS: ???";
	lblFPS->foreColor = { 255, 255, 255 };
	ui.add(lblFPS);

	// resolution label
	lblWindowSize = new Label();
	lblWindowSize->x = 10; 
	lblWindowSize->y = 30;
	lblWindowSize->text = "Window size: ???";
	lblWindowSize->foreColor = { 255, 255, 255 };
	ui.add(lblWindowSize);

	// mouse cursor position label
	lblCursorPos = new Label();
	lblCursorPos->x = 10; 
	lblCursorPos->y = 50;
	lblCursorPos->text = "Cursor position: ";
	lblCursorPos->foreColor = { 255, 255, 255 };
	ui.add(lblCursorPos);

	// aesthetically positioned buttons
	Button* btn0 = new Button();
	Button* btn1 = new Button();
	Button* btn2 = new Button();
	Button* btn3 = new Button();
	btn0->setBounds(80,     80    , 80 * 6,  80 * 2); btn0->setBackColor(255, 128, 128); btn0->setText("0"); ui.add(btn0);
	btn1->setBounds(80,     80 * 4, 80 * 6,  80 * 2); btn1->setBackColor(128, 255, 128); btn1->setText("1"); ui.add(btn1);
	btn2->setBounds(80,     80 * 7, 80 * 14, 80    ); btn2->setBackColor(128, 128, 255); btn2->setText("2"); ui.add(btn2);
	btn3->setBounds(80 * 8, 80    , 80 * 7 , 80 * 5); btn3->setBackColor(255, 128, 255); btn3->setText("3"); ui.add(btn3);
}
