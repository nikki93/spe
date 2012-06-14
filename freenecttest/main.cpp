#include "ofAppRunner.h"
#include "ofAppGlutWindow.h"

#include "App.h"

int main()
{
    ofAppGlutWindow window;
    ofSetupOpenGL(&window, 1024,768, OF_WINDOW);

    ofRunApp(new App());
}
