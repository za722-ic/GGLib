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
	const int UI_X = 250;
	const int UI_Y = 50;
	size_t NUM_COLS = 3, NUM_ROWS = 3;
	cb.setBounds(UI_X, UI_Y + 550, 170, 30);
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

	return true;
}

void Demo::onGameQuit()
{
	// Release font file
	// TODO: isn't this dealt with automatically by the asset manager's destructor? why unload here?
	assetManager.unloadFont("Beef-d.ttf");
}

// TODO: the case where r is greater than w or h makes this look weird --> how does CSS handle it?
void renderRoundedRect(SDL_Renderer* renderer, int x, int y, int w, int h, int r, SDL_Color color = { 255, 255, 255, 255 }, const unsigned int trianglesPerCorner = 32)
{
	std::vector<SDL_Vertex> verts;

	const float INTERVAL = 0.5f * M_PI / trianglesPerCorner; // angle between triangles in a given corner

	auto appendRoundedCornerVerts = [&](float cx, float cy, float startingAngle)
	{
		float angle = startingAngle;

		for (int i = 0; i < trianglesPerCorner; i++)
		{
			SDL_Vertex v1 =
			{
				SDL_FPoint{cx + r * cosf(angle), cy + r * sinf(angle)},
				color,
				SDL_FPoint { 0 }
			};
			
			angle += INTERVAL;

			SDL_Vertex v2 =
			{
				SDL_FPoint{cx + r * cosf(angle), cy + r * sinf(angle)},
				color,
				SDL_FPoint { 0 }
			};

			SDL_Vertex v3 =
			{
				SDL_FPoint{cx, cy},
				color,
				SDL_FPoint { 0 }
			};

			verts.push_back(v1);
			verts.push_back(v2);
			verts.push_back(v3);
		}
	};

	appendRoundedCornerVerts(x + w - r, y + h - r, 0          ); // top right
	appendRoundedCornerVerts(x + r,     y + h - r, 0.5f * M_PI); // top left
	appendRoundedCornerVerts(x + r,     y + r,     M_PI       ); // bottom left
	appendRoundedCornerVerts(x + w - r, y + r,     1.5f * M_PI); // bottom right

	SDL_RenderGeometry(renderer, nullptr, verts.data(), verts.size(), nullptr, 0);

	// render rects for everything that's not the corners

	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

	SDL_Rect rTop = { x + r, y, w - 2 * r, r };
	SDL_RenderFillRect(renderer, &rTop);

	SDL_Rect rMid = { x, y+r, w, h - 2 * r };
	SDL_RenderFillRect(renderer, &rMid);

	SDL_Rect rBot = { x + r, y + h - r, w - 2 * r, r };
	SDL_RenderFillRect(renderer, &rBot);
}

void renderRegularPolygon(SDL_Renderer* renderer, int x, int y, unsigned int numVertices = 3, float rotAngle = 0, float scale = 250.0f, SDL_Color color = {255, 255, 255, 255})
{
	const float INTERVAL = (2.0f * M_PI) / numVertices;

	std::vector<SDL_Vertex> verts;

	for (int i = 0; i < numVertices; i++)
	{
		float angle = rotAngle + INTERVAL * i;
	
		SDL_Vertex v1 =
		{
			SDL_FPoint{x + scale * cosf(angle), y + scale * sinf(angle)},
			color,
			SDL_FPoint { 0 }
		};
		
		angle = rotAngle + INTERVAL * (i + 1);

		SDL_Vertex v2 =
		{
			SDL_FPoint{x + scale * cosf(angle), y + scale * sinf(angle)},
			color,
			SDL_FPoint { 0 }
		};

		SDL_Vertex v3 =
		{
			SDL_FPoint{(float)x, (float)y},
			color,
			SDL_FPoint { 0 }
		};
	
		verts.push_back(v1);
		verts.push_back(v2);
		verts.push_back(v3);
	}

	SDL_RenderGeometry(renderer, nullptr, verts.data(), verts.size(), nullptr, 0);
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

	triRotAngle += deltaTime();
	renderRegularPolygon(canvas.getSDLRenderer(), 800, 300, 3, triRotAngle, 45.0f, {255, 0, 255, 255});
	renderRegularPolygon(canvas.getSDLRenderer(), 800, 450, 4, triRotAngle, 45.0f, {255, 0, 255, 255});
	renderRegularPolygon(canvas.getSDLRenderer(), 800, 600, 5, triRotAngle, 45.0f, {255, 0, 255, 255});
	renderRegularPolygon(canvas.getSDLRenderer(), 800, 750, 6, triRotAngle, 45.0f, {255, 0, 255, 255});
	    
	renderRoundedRect(canvas.getSDLRenderer(), 900, 200, 300, 300, 20);
	renderRoundedRect(canvas.getSDLRenderer(), 900, 600, 300, 150, 75, {128, 128, 255, 255});

	// TODO: would be beneficial to abstract this away from the user. problem is you would still need to rely on the user clearing the screen and updating the screen --> how much should the user be able to control ui rendering? ideally as much as they wish, or as little as they wish...
	uiManager.renderUI(&canvas);


	y += vy * deltaTime();
	if (y + 20 > canvas.getHeight())
	{
		y = canvas.getHeight() - 20;
		vy = -vy;
	}
	if (y < 0)
	{
		y = 0;
		vy = -vy;
	}
	renderRegularPolygon(canvas.getSDLRenderer(), 1000, y, 64, 0, 20.0f, {255, 255, 0, 255});
	

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
