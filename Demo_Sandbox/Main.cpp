#include "GG/Core/Application.h"

class MyApp : public GG::Application
{
	bool onInit() override
	{
		// set up window
		window.setTitle("MyApp");
		window.setResizable(false);
		window.setSize(300, 300);

		// tell library set up was successful
		return true;
	}

	void onLoop() override
	{
		// clear screen
		canvas.setColor(0x58, 0x5B, 0x65);
		canvas.clear();

		// draw UI
		canvas.setColor(255, 0, 0);
		canvas.fillRect(100, 100, 100, 100);

		// update screen
		canvas.present();
	}
};

int main(int argc, char* args[])
{
	MyApp app;
	app.begin();

	return 0;
}