#include "App.h"

#include <ofGraphics.h>
#include <ofAppRunner.h>
#include <ofImage.h>

#include "Globals.h"

Brush *tempBrush()
{
    return new Brush(Globals::mousePos, ofVec2f(0, 0),
            ofColor(255, 0, 0), 8,
            200, 0.2);
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

void gradientField(Field &field, const ofPixels &pix)
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

            field.set(i, j, 50*g);
        }
}

//--------------------------------------------------------------
App::App()
    : _forceField(1024, 768)
{
}
//--------------------------------------------------------------
void App::setup()
{
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofSetBackgroundAuto(false);

    Globals::mousePos = ofVec2f(0, 0);

    // force field
    //_forcePotential.loadImage("/home/nikki/Development/Projects/spe/brushtest/data/somepicture.png");
    _forcePotential.loadImage("depth.png");
    const ofPixels &pix = _forcePotential.getPixelsRef();
    gradientField(_forceField, pix);

    // color image
    _color.loadImage("color.png");

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

        if (!brush->move(_forceField.get(i, j), 0.005))
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
    // draw brushes
    for (BrushList::iterator i = _brushes.begin();
            i != _brushes.end(); ++i)
        (*i)->draw();
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

    // draw force field
    ofSetColor(ofColor::black);
    _forceField.draw(ofVec2f(0, 0), 7);
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
            _brushes.push_back(tempBrush());
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
        Globals::mouseLeft = false;
}
//--------------------------------------------------------------
