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
	root->setX(0);
	root->setY(0);
	root->setWidthAbs(500);
	root->setHeightAbs(500);
	root->setColor(0xeeaaaaff);
	root->setPadding(10);
	
	// set up file dialog
	openFileDialog.onDialogClose = [](const std::vector<std::string> filePaths)
	{
		if (filePaths.empty()) std::cout << "NO FILEPATHS" << std::endl;
		for (auto filePath : filePaths) std::cout << filePath << std::endl;
	};
	openFileDialog.acceptMultiple = true;

	// create button to trigger file dialog
	Button *btnFileDialog = new Button;
	btnFileDialog->setText("Open file");
	btnFileDialog->setColor(0xffffffff);
	btnFileDialog->setOnClick([&](){
		openFileDialog.showDialog(ggWindow.getSDLWindow());
	});
	root->add(btnFileDialog);
}
