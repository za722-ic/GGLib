
#include "GG/Core/Application.h"

class MyApp : public GG::Application
{
    bool onInit() override
    {
        return true;
    }

    void onQuit() override
    {
    }

    void onLoop() override
    {
        // clear screen
        canvas.setColor(0x58, 0x5B, 0x65);
        canvas.clear();

        // draw a red square
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
