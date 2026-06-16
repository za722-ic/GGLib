# GGLib

## Getting Started

- Install CMake, Git
- Clone the repo (this might take a while as it recursively clones dependencies):

``` bash
git clone --recursive https://www.github.com/za722-ic/GGLib -b report
```

- Run CMake:

``` bash
cd GGLib
mkdir build
cd build
cmake ..
```

### Windows

A `.sln` file will have been generated in the `build` folder. Open it using Visual Studio.

Click the green "Local Windows Debugger" button to build and run one of the example projects. Set the project to "Release" mode to for good performance, or "Debug" if you want to use the debugger.

You can select other example projects by opening "Solution Explorer" (CTRL+ALT+L), and then right-clicking one of the demo projects ("Demo_\*"), and then clicking "Set as Startup Project". The demos should give a good idea how to use the library.

### Linux

On Linux, run the following in the `build` folder:

``` bash
make
```

This will build all the demo applications. You can then go into the respective folder and run the generated executable. The demos should give a good idea how to use the library.

> Note: There are a few example programs that are Windows only. See `Examples/CMakeLists.txt`.

## Creating an application

Here is a small project that creates an application with a counter:

``` cpp
#include "GG/Core/Application.h"

class MyApp : public GG::Application
{
private:
	GG::RootContainer* root = nullptr;

private:
	bool onInit() override
	{
		// load font
		assetManager.setBaseAssetPath("resources/");
		assetManager.loadFont("NotoSans-Medium.ttf", 20);

		// init text subsystems
		canvas.setFont(assetManager.getFont("NotoSans-Medium.ttf").value());
		GG::Text::init(canvas.getSDLRenderer(), assetManager.getFont("NotoSans-Medium.ttf").value());

		// define UI
		defineUI();

		// tell library set up was successful
		return true;
	}

	void onQuit() override
	{
		// close text subsystem
		GG::Text::close();

		// release the UI
		root->destroySelfAndChildren();
	}

	void onLoop() override
	{
		// clear screen
		canvas.setColor(0x58, 0x5B, 0x65);
		canvas.clear();

		// set title to FPS
		window.setTitle("FPS: " + std::to_string(averageFPS()));

		// layout and render UI
		root->calculateLayout(0, 0, window.getWidth(), window.getHeight());
		root->render(&canvas);

		// update screen
		canvas.present();
	}

private:
	void defineUI()
	{
		// set up window
		window.setTitle("MyApp");
		window.setResizable(false);
		window.centerWindowPosition();
		window.setSize(640, 480);

		// define root container
		root = new GG::RootContainer;
		root->layoutDirection = GG::LayoutDirection::TOP_TO_BOTTOM;
		root->setPadding(30);
		root->setChildGap(30);
		root->setInputManager(&inputManager);

		// create label
		GG::Label* label = new GG::Label("0");
		label->setHAlignment(GG::HAlignmentMode::CENTER); // center align label text horizontally
		root->add(label);

		// create button
		GG::Button* button = new GG::Button("Click me!");
		button->setOnClick([=]() {
			int numClicks = std::stoi(label->getText());
			label->setText(std::to_string(numClicks + 1));
		});
		button->setMaxHeight(80);
		button->setMaxWidth(300);
		root->add(button);
	}
};

int main(int argc, char* args[])
{
	MyApp app;
	app.begin();

	return 0;
}
```

## Drawing commands

You can draw various items directly to the window. For a full list of draw commands, see `GGLib/include/GG/Rendering/Canvas.h`.

## Creating a UI

To create a UI, you must construct a UI hierarchy using container and control objects arranged in a tree like structure. Then, in the `onLoop()` function, you can issue calls to layout and render the UI hierarchy. See the above example.

The root node must be of type `GG::RootContainer`. Only `GG::RootContainer` and `GG::Container` can have children in the hierarchy.

You can create various controls. The following are built-in:
- `GG::Button`
- `GG::Checkbox`
- `GG::Combobox`
- `GG::HorizontalDivider`
- `GG::HorizontalSpacer`
- `GG::Label`
- `GG::Slider`
- `GG::Textbox`
- `GG::Toggle`

You can also create custom controls by inheriting the `GG::Control` class. See `Demo_Noise` for an example.
