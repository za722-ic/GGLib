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

const int UI_X = 250;
const int UI_Y = 400;

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
	size_t NUM_COLS = 3, NUM_ROWS = 3;
	cb.setBounds(UI_X, UI_Y + 200, 170, 30);
	cb.setText("Checkbox");
	Checkbox* cbPtr = &cb; // TODO: ugly
	for (size_t c = 0; c < NUM_COLS; c++)
	{
		for (size_t r = 0; r < NUM_ROWS; r++)
		{
			size_t i = r * NUM_COLS + c;

			Button *btn = new Button;

			btn->setBounds(c * 50 + UI_X, r*50 + UI_Y, 45, 45);
			btn->setBackColor(128, 255, 128);
			btn->setOnMouseEnter([=]() { btn->setBackColor(255, 255, 255); });
			btn->setOnMouseExit([=]() {btn->setBackColor(128, 255, 128); });
			btn->setOnClick([=]() { 
				if (cbPtr->isChecked)
				{
					int i = stoi(btn->getText());
					i++;
					btn->setText(std::to_string(i));
				}
			});
			btn->setText(std::to_string(i));

			buttons.push_back(btn);
		}
	}
	slider.x = UI_X;
	slider.y = UI_Y + 300;
	slider.max = 1000;
	tb.setBounds(UI_X, UI_Y + 500, 200, 40);
	combobox.setBounds(UI_X + 700, UI_Y + 300, 200, 40);

	return true;
}

void Demo::onGameQuit()
{}


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
	canvas.drawString("FPS: " + std::to_string(avgFps), 10, 90);

	canvas.setColor(170);
	canvas.renderRoundedRect(UI_X + 950, 200, 300, 300, 20);

	canvas.setColor(255, 128, 255);
	canvas.renderRoundedRect(UI_X + 800 - 45*2-10, 200, 200, 650, 50);

	triRotAngle += deltaTime();
	canvas.setColor(255, 0, 255);
	canvas.renderRegularPolygon(UI_X + 800, 300, 3, triRotAngle, 45.0f);
	canvas.renderRegularPolygon(UI_X + 800, 450, 4, triRotAngle, 45.0f);
	canvas.renderRegularPolygon(UI_X + 800, 600, 5, triRotAngle, 45.0f);
	canvas.renderRegularPolygon(UI_X + 800, 750, 6, triRotAngle, 45.0f);
	    

	// TODO: would be beneficial to abstract this away from the user. problem is you would still need to rely on the user clearing the screen and updating the screen --> how much should the user be able to control ui rendering? ideally as much as they wish, or as little as they wish...
	canvas.setColor(255); // TODO: this should not be a thing --> ui elements' color should be defined by ui elements
	uiManager.renderUI(&canvas);


	y += vy * deltaTime();
	if (y + 20 > canvas.getHeight())
	{
		y = canvas.getHeight() - 20;
		vy = -vy;
	}
	if (y < 20)
	{
		y = 20;
		vy = -vy;
	}
	
	canvas.setColor(255, 255, 0);
	canvas.renderRegularPolygon(1700, y, 64, 0, 20.0f);
	

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
