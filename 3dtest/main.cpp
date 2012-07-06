#include "ofAppRunner.h"
#include "ofAppGlutWindow.h"

#include "App.h"

int main()
{
    ofAppGlutWindow window;
    ofSetupOpenGL(&window, 1324,768, OF_WINDOW);

    ofRunApp(new App());

    return 0;
}
