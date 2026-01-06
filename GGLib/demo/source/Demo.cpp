#include "Demo.h"

bool Demo::onGameInit()
{
	// load assets
	if (!demoAssetManager->loadResources())
	{
		std::cout << "Failed to load game resources! See console output. Aborting game." << std::endl;
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
	btn.setBounds(500, 500, 50, 50);
	btn.setColor(128, 255, 128);
	btn.setOnMouseEnter([&]() {btn.setColor(255, 255, 255); });
	btn.setOnMouseExit([&]() {btn.setColor(128, 255, 128); });
	btn.setOnClick([]() { std::cout << "Hello" << std::endl; });

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

	// render button
	btn.render(&canvas);

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
