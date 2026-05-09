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
	defineElements();

	return true;
}

void Demo::onQuit()
{
}

float duration = 10.0;
float endAngle = 2*M_PI;
float vel = 2*M_PI / 10;
void Demo::onLoop()
{
	// clear screen
	canvas.setColor(0x58, 0x5B, 0x65);
	canvas.clear();

	// update the ring size
	if (deltaTime() > 0)
		endAngle -= vel * deltaTime();
	
	// draw the ring 
	canvas.setColor(255, 0, 255, 255);
	canvas.drawRect(300, 300, 300, 300);
	canvas.setColor(255,128);
	canvas.drawArc(300,300, 300, 300, 0, endAngle, 64, 64);
	canvas.setAlignment(Canvas::Alignment::CENTER_CENTER);
	canvas.drawString(std::to_string(endAngle), 450, 450);
	canvas.setAlignment(Canvas::Alignment::TOP_LEFT);

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

void Demo::defineElements()
{
}
