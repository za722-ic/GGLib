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
	fpsLabel.text = "FPS: " + std::to_string(avgFps);

	// get updated window resolution
	std::string windowResolutionText = "Window size: " + std::to_string(ggWindow.getWidth()) + "x" + std::to_string(ggWindow.getHeight());
	resolutionLabel.setText(windowResolutionText);

	// get updated cursor position
	std::string cursorPosText = "Cursor position: " + std::to_string(inputManager.getMouseX()) + ", " + std::to_string(inputManager.getMouseY());
	cursorPosLabel.setText(cursorPosText);


	// TODO: would be beneficial to abstract this away from the user. problem is you would still need to rely on the user clearing the screen and updating the screen --> how much should the user be able to control ui rendering? ideally as much as they wish, or as little as they wish...
	canvas.setColor(255); // TODO: this should not be a thing --> ui elements' color should be defined by ui elements
	uiManager.renderUI(&canvas);

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
	// (FPS) label
	fpsLabel.x = 10; 
	fpsLabel.y = 10;
	fpsLabel.text = "FPS: ???";
	fpsLabel.foreColor = { 255, 255, 255 };

	// Resolution label
	resolutionLabel.x = 10; 
	resolutionLabel.y = 30;
	resolutionLabel.text = "Window size: ???";
	resolutionLabel.foreColor = { 255, 255, 255 };

	// Mouse cursor position label
	cursorPosLabel.x = 10; 
	cursorPosLabel.y = 50;
	cursorPosLabel.text = "Cursor position: ";
	cursorPosLabel.foreColor = { 255, 255, 255 };

	btn0.setBounds(80,     80    , 80 * 6,  80 * 2); btn0.setBackColor(255, 128, 128); btn0.setText("0");
	btn1.setBounds(80,     80 * 4, 80 * 6,  80 * 2); btn1.setBackColor(128, 255, 128); btn1.setText("1");
	btn2.setBounds(80,     80 * 7, 80 * 14, 80    ); btn2.setBackColor(128, 128, 255); btn2.setText("2");
	btn3.setBounds(80 * 8, 80    , 80 * 7 , 80 * 5); btn3.setBackColor(255, 128, 255); btn3.setText("3");

}
//void Demo::initUI()
//{
//	// (FPS) label
//	fpsLabel.x = 10; 
//	fpsLabel.y = 10;
//	fpsLabel.text = "FPS: ???";
//	fpsLabel.foreColor = { 255, 255, 255 };
//
//	// Resolution label
//	resolutionLabel.x = 10; 
//	resolutionLabel.y = 30;
//	resolutionLabel.text = "Window size: ???";
//	resolutionLabel.foreColor = { 255, 255, 255 };
//
//	// Mouse cursor position label
//	cursorPosLabel.x = 10; 
//	cursorPosLabel.y = 50;
//	cursorPosLabel.text = "Cursor position: ";
//	cursorPosLabel.foreColor = { 255, 255, 255 };
//
//	// root location for the rest of the UI elements
//	const int UI_X = 250;
//	const int UI_Y = 100;
//
//	// checkbox
//	cb.setBounds(UI_X, UI_Y, 170, 30);
//	cb.setText("Checkbox");
//
//	// button
//	btn.setBounds(UI_X, UI_Y + 200, 200, 50);
//	btn.setBackColor(128, 255, 128);
//	btn.setOnMouseEnter([=]() { btn.setBackColor(255, 255, 255); });
//	btn.setOnMouseExit([=]() {btn.setBackColor(128, 255, 128); });
//	btn.setOnClick([=]() { 
//		int i = stoi(btn.getText());
//		i++;
//		btn.setText(std::to_string(i));
//	});
//	btn.setText("0");
//
//	// slider
//	slider.x = UI_X;
//	slider.y = UI_Y + 400;
//	slider.max = 1000;
//
//	// textbox
//	tb.setBounds(UI_X, UI_Y + 600, 200, 40);
//
//	// combobox
//	combobox.setBounds(UI_X, UI_Y + 800, 200, 40);
//
//}
