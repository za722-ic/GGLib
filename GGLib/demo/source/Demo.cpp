#include "Demo.h"

bool Demo::onGameInit()
{
	// load assets
	if (!demoAssetManager->loadResources())
	{
		std::cout << "Failed to load resources! See console output. Aborting." << std::endl;
		return false;
	}

	// set up window
	gameWindow.setTitle("Demo");
	gameWindow.setFullScreen(false);
	gameWindow.setResizable(true);
	gameWindow.setMouseHidden(false);

	// tell canvas to use loaded font
	canvas.setFont(demoAssetManager->getFont());

	// subscribe to mouse and keyboard events
	inputManager.addKeyEventListener(this);
	inputManager.addMouseEventListener(this);

	// init UI
	size_t NUM_COLS = 10, NUM_ROWS = 10;
	for (size_t c = 0; c < NUM_COLS; c++)
	{
		for (size_t r = 0; r < NUM_ROWS; r++)
		{
			size_t i = r * NUM_COLS + c;

			Button *btn = new Button;

			btn->setBounds(c * 50 + 250, r*50 + 250, 45, 45);
			btn->setBackColor(128, 255, 128);
			btn->setOnMouseEnter([=]() { btn->setBackColor(255, 255, 255); });
			btn->setOnMouseExit([=]() {btn->setBackColor(128, 255, 128); });
			btn->setOnClick([=]() { 
				int i = stoi(btn->getText());
				i++;
				btn->setText(std::to_string(i));
			});
			btn->setText(std::to_string(i));

			buttons.push_back(btn);
		}
	}

	return true;
}

void Demo::onGameQuit()
{
	// Release font file
	// TODO: isn't this dealt with automatically by the asset manager's destructor? why unload here?
	assetManager.unloadFont("Beef-d.ttf");
}

void Demo::onGameLoop()
{
	// clear screen
	canvas.setColor(30);
	canvas.clear();
	
	// draw text
	canvas.resetOrigin();
	canvas.setAlignment(Canvas::Alignment::TOP_LEFT);
	canvas.setColor(220, 220, 220);
	canvas.drawString("Canvas width: " + std::to_string(canvas.getWidth()), 10, 10);
	canvas.drawString("Canvas height: " + std::to_string(canvas.getHeight()), 10, 30);
	canvas.drawString("Mouse X: " + std::to_string(inputManager.getMouseX()), 10, 50);
	canvas.drawString("Mouse Y: " + std::to_string(inputManager.getMouseY()), 10, 70);

	// TODO: would be beneficial to abstract this away from the user. problem is you would still need to rely on the user clearing the screen and updating the screen --> how much should the user be able to control ui rendering? ideally as much as they wish, or as little as they wish...
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
		gameWindow.toggleFullScreen();
	}
}

void Demo::onMouseEvent(MouseEventType mouseEventType, int mouseX, int mouseY)
{
}
