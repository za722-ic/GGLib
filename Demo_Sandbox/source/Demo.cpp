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

void Demo::onLoop()
{
	// clear screen
	canvas.setColor(0x58, 0x5B, 0x65);
	canvas.clear();

	// draw UI
	root->setWidthAbs(ggWindow.getWidth());
	root->setHeightAbs(ggWindow.getHeight());
	root->calculateLayout();
	root->render(&canvas);

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
	// TODO: automate this
	Element::inputManager = &inputManager;

	// create root container
	root = new Container;
	root->setColor(0xeeaaaaff);
	root->radius = 0;
	root->layoutMode = LayoutMode::FLEX;
	root->horizontalAlignmentMode = HAlignmentMode::CENTER;
	root->verticalAlignmentMode = VAlignmentMode::CENTER;
	root->layoutDirection = LayoutDirection::LEFT_TO_RIGHT;
	root->setPadding(60);
	root->setChildGap(60);

	Container* buffer = new Container;
	buffer->isVisible = true;
	buffer->setColor(0xFF00FFFF);
	root->add(buffer);

	// create button container 
	Container* absContainer = new Container;
	absContainer->setColor(0xeeaaeeff);
	absContainer->layoutMode = LayoutMode::ABSOLUTE;
	//absContainer->setWidthAbs(1280);
	//absContainer->setHeightAbs(720);
	absContainer->horizontalAutosize = true;
	absContainer->verticalAutosize = true;
	absContainer->setMinWidth(600);
	root->add(absContainer);

	Container* absChildThatIsFlexContainer = new Container;
	absChildThatIsFlexContainer->setColor(0xff0000ff);
	absChildThatIsFlexContainer->layoutMode = LayoutMode::FLEX;
	absChildThatIsFlexContainer->setWidthAbs(300);
	absChildThatIsFlexContainer->setHeightAbs(300);
	absChildThatIsFlexContainer->setXAbs(0);
	absChildThatIsFlexContainer->setYAbs(0);
	absContainer->add(absChildThatIsFlexContainer);
	
	
	// create buttons
	for (size_t i = 1; i < 3; i++)
	{
		Button* button = new Button;
		button->setText(std::to_string(i));
		button->backColor = { (unsigned char)MoreMath::random(0, 255), (unsigned char)MoreMath::random(0, 255), (unsigned char)MoreMath::random(0, 255), 255 };

		button->setWidthAbs( 300);
		button->setHeightAbs(300);
		button->setXAbs(     i*300);
		button->setYAbs(     i*300);

		button->setOnClick([=]()
		{
			button->setXAbs(button->xAbs == 0 ? i*300 : 0);
		});

		absContainer->add(button);
	}

	// create buttons
	for (size_t i = 0; i < 6; i++)
	{
		Button* button = new Button;
		button->setText(std::to_string(i));
		button->backColor = { 0xFF, 0xA5, 0x00, 0xFF };

		button->setWidthAbs( MoreMath::random(30, 90));

		absChildThatIsFlexContainer->add(button);
	}
}
