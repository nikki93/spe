#include "App.h"

#include <ofGraphics.h>
#include <ofAppRunner.h>
#include <ofImage.h>
#include <unistd.h>

#include "Globals.h"

Brush *tempBrush()
{
    return new Brush(Globals::mousePos, ofVec2f(0, 0),
            ofColor(255, 0, 0), 10,
            1000, 0.05);
}

void circleField(ofVec2f field[1024][768])
{
    for (int i = 0; i < 1024; ++i)
        for (int j = 0; j < 768; ++j)
        {
            float x = i - 512;
            float y = j - 384;
            field[i][j] = ofVec2f(-y, x);
        }
}

void gradientField(ofVec2f field[1024][768], const ofPixels &pix)
{
#define VAL(x, y) pix.getColor(x, y).r
    for (int i = 1; i < 1023; ++i)
        for (int j = 1; j < 767; ++j)
        {
            ofVec2f g(0, 0);

            // left
            g.x -= 2 * VAL(i - 1, j);
            g.x -= VAL(i - 1, j - 1);
            g.x -= VAL(i - 1, j + 1);
            // right
            g.x += 2 * VAL(i + 1, j);
            g.x += VAL(i + 1, j - 1);
            g.x += VAL(i + 1, j + 1);

            // up
            g.y -= 2 * VAL(i, j - 1);
            g.y -= VAL(i - 1, j - 1);
            g.y -= VAL(i + 1, j - 1);
            // down
            g.y += 2 * VAL(i, j + 1);
            g.y += VAL(i - 1, j + 1);
            g.y += VAL(i + 1, j + 1);

            field[i][j] = 50*g;
        }
}

//--------------------------------------------------------------
void App::setup()
{
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);

    char path[200];
    getcwd(path, 200);
    printf("%s\n", path);

    // don't clear each frame
    ofSetBackgroundAuto(false);

    // initial mouse position
    Globals::mousePos = ofVec2f(0, 0);

    // make force field
    //_forcePotential.loadImage("/home/nikki/Development/Projects/spe/brushtest/data/somepicture.png");
    _forcePotential.loadImage("somepicture.png");
    const ofPixels &pix = _forcePotential.getPixelsRef();
    gradientField(_forceField, pix);

    // draw background
    clear();
}
//--------------------------------------------------------------
void App::update()
{
    float elapsed = ofGetLastFrameTime();

    // move the brush!
    for (BrushList::iterator iter = _brushes.begin();
            iter != _brushes.end(); )
    {
        Brush *brush = *iter;

        ofVec2f pos = brush->getPosition();
        int i = ofClamp(pos.x, 0, 1023);
        int j = ofClamp(pos.y, 0, 767);

        if (!brush->move(_forceField[i][j], 0.005))
        {
            delete brush;
            iter = _brushes.erase(iter);
        }
        else
            ++iter;
    }
}
//--------------------------------------------------------------
void App::draw()
{
    // see the brush!
    for (BrushList::iterator i = _brushes.begin();
            i != _brushes.end(); ++i)
        (*i)->draw();

    ofSetColor(ofColor::blue);
    for (int i = 0; i < 1024; i += 15)
        for (int j = 0; j < 768; j += 15)
        {
            ofVec2f start(i, j);
            ofLine(start, start + 10*(_forceField[i][j].normalized()));
            ofCircle(start, 1);
        }
}
//--------------------------------------------------------------
void App::clear()
{
    while (!_brushes.empty())
    {
        delete _brushes.back();
        _brushes.pop_back();
    }

    ofSetColor(ofColor::white);
    _forcePotential.draw(0, 0);
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

    if (button == 0)
    {
        // put brush down
        _brushes.push_back(tempBrush());
    }
}
//--------------------------------------------------------------
void App::mousePressed(int x, int y, int button)
{
    switch (button)
    {
        case 0:
            Globals::mouseLeft = true;
            break;

        case 2:
            clear();
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
