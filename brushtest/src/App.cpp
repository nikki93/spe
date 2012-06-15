#include "App.h"

#include <ofGraphics.h>
#include <ofAppRunner.h>

#include "Globals.h"

Brush *tempBrush()
{
    return new Brush(Globals::mousePos, ofVec2f(0, 0),
            ofColor(255, 0, 0), 10,
            600, 0.7);
}

//--------------------------------------------------------------
void App::setup()
{
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofBackground(ofColor::white);

    // don't clear each frame
    ofSetBackgroundAuto(false);

    // initial mouse position
    Globals::mousePos = ofVec2f(0, 0);

    // make a brush!
    _testBrush = tempBrush();
}
//--------------------------------------------------------------
void App::update()
{
    float elapsed = ofGetLastFrameTime();

    // move the brush!
    if (_testBrush)
        if (!_testBrush->move(ofVec2f(200, 0), 0.01))
        {
            delete _testBrush;
            _testBrush = 0;
        }
}
//--------------------------------------------------------------
void App::draw()
{
    // see the brush!
    if (_testBrush)
        _testBrush->draw();
}
//--------------------------------------------------------------
void App::keyPressed(int key)
{

}
//--------------------------------------------------------------
void App::keyReleased(int key)
{

}
//--------------------------------------------------------------
void App::mouseMoved(int x, int y )
{
    Globals::mousePos.x = x;
    Globals::mousePos.y = y;
}
//--------------------------------------------------------------
void App::mouseDragged(int x, int y, int button)
{
    Globals::mousePos.x = x;
    Globals::mousePos.y = y;
}
//--------------------------------------------------------------
void App::mousePressed(int x, int y, int button)
{
    switch (button)
    {
        case 0:
            Globals::mouseLeft = true;

            // put brush down
            if (!_testBrush)
                _testBrush = tempBrush();
            break;

        case 2:
            ofBackground(ofColor::white);
            break;
    }
}
//--------------------------------------------------------------
void App::mouseReleased(int x, int y, int button)
{
    if (button == 0)
    {
        Globals::mouseLeft = false;

        // lift brush
        delete _testBrush;
        _testBrush = 0;
    }
}
//--------------------------------------------------------------
void App::windowResized(int w, int h)
{

}
//--------------------------------------------------------------
