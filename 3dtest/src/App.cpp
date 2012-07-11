#include "App.h"

#include <ofGraphics.h>
#include <of3dUtils.h>
#include <ofAppRunner.h>

#include "Frame.h"
#include "Settings.h"
#include "GUI.h"

static void drawQuad(const ofVec2f &start, const ofVec2f &size, const ofVec2f &texSize)
{
    ofVec2f end = start + size;
    glBegin(GL_QUADS);  
        glTexCoord2f(0,         0        ); glVertex3f(start.x, start.y, 0);  
        glTexCoord2f(texSize.x, 0        ); glVertex3f(end.x,   start.y, 0);  
        glTexCoord2f(texSize.x, texSize.y); glVertex3f(end.x,   end.y,   0);  
        glTexCoord2f(0,         texSize.y); glVertex3f(start.x, end.y,   0);  
    glEnd();   
}

//--------------------------------------------------------------
App::App()
{
}
//--------------------------------------------------------------
void App::setup()
{
    // gui
    _gui = new GUI(*this);

    // renderer setup
    _frame = new Frame(*this);
    _timer = 0.5;
    _timerSet = true;

    // camera
    _cam.setPosition(ofVec3f(80, 100, 80));
    _cam.lookAt(ofVec3f(0, 50, 0));

    // light
    _light.enable();
    _light.setPointLight();
    _light.setDiffuseColor(ofFloatColor(0.8, 0.5, 0.1));
    _light.setPosition(40, 40, 40);

    // balls
    int n = 0;//10;
    while (n--)
        _balls.push_back(new Ball(
                    ofVec3f(ofRandom(-100, 100), ofRandom(20, 70), ofRandom(-100, 100)), // pos
                    ofVec3f(0, ofRandom(-5, 5), 0), // vel
                    ofVec3f(0, -20, 0), // acc
                    ofColor(ofRandom(0, 255), ofRandom(0, 255), ofRandom(0, 255)), // col
                    ofRandom(4, 14) // radius
                    ));

    // models
    //Model *m = new Model("data/models/dolphin/dolphin_000001.obj", 1,
            //ofMatrix4x4::newScaleMatrix(20, 20, 20));

    ofDisableArbTex();

    Model *m = new Model("data/models/ram/ram.obj", 1,
            ofMatrix4x4::newScaleMatrix(75, 75, 75));
    _tex.loadImage("models/ram/ram.jpg");
    m->tex = &(_tex.getTextureReference());
    m->pos = ofVec3f(0, 10, 0);
    _models.push_back(m);

    // floor
    _floorTex.loadImage("floor.jpg");
}
//--------------------------------------------------------------
void App::update()
{
    if (_timer <= 0)
    {
        if (Settings::autoUpdate)
            newFrame();
    }
    else if (_timerSet)
        _timer -= ofGetLastFrameTime();
    else
    {
        // brush rendering
        bool more = _frame->moveBrushes();
        _frame->drawBrushes();
        _frame->combineFrame();

        if (!more)
        {
            _timer = Settings::updateTime;
            _timerSet = true;
        }
    }
}
//--------------------------------------------------------------
void App::stepScene(float elapsed)
{
    // camera rotation
    ofVec3f camPos = _cam.getPosition();
    camPos.rotate(30 * elapsed, ofVec3f(0, 1, 0));
    _cam.setPosition(camPos);
    _cam.lookAt(ofVec3f(0, 50, 0));

    // balls
    for (std::vector<Ball *>::iterator i = _balls.begin();
            i != _balls.end(); ++i)
        (*i)->update(elapsed);

    // model animation
    for (std::vector<Model *>::iterator i = _models.begin();
            i != _models.end(); ++i)
        (*i)->update(elapsed);
}
//--------------------------------------------------------------
void App::draw()
{
    _frame->debugDraw();
    ofSetColor(ofColor::black);
}
//--------------------------------------------------------------
void App::drawScene()
{
    _cam.begin();
    glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_ENABLE_BIT);

    // clear
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    ofDisableAlphaBlending();
    ofEnableLighting();
    ofBackground(ofColor::white);

    // axis
    /*
    ofSetColor(ofColor::gray);
    ofSphere(2);
    ofDrawAxis(20);
    */

    // models, balls
    for (std::vector<Model *>::iterator i = _models.begin();
            i != _models.end(); ++i)
        (*i)->draw();

    for (std::vector<Ball *>::iterator i = _balls.begin();
            i != _balls.end(); ++i)
        (*i)->draw();

    // floor
    /*
    _floorTex.getTextureReference().bind();
    glBegin(GL_QUADS);
        glTexCoord2f(1, 1); glNormal3f(0, 1, 0); glVertex3f( 100, 0,  100);
        glTexCoord2f(1, 0); glNormal3f(0, 1, 0); glVertex3f( 100, 0, -100);
        glTexCoord2f(0, 0); glNormal3f(0, 1, 0); glVertex3f(-100, 0, -100);
        glTexCoord2f(0, 1); glNormal3f(0, 1, 0); glVertex3f(-100, 0,  100);
    glEnd();
    _floorTex.getTextureReference().unbind();
    */

    glPopAttrib();
    _cam.end();
}
//--------------------------------------------------------------
void App::exit()
{
    // remove gui
    delete _gui;

    // remove models
    while (!_models.empty())
    {
        delete _models.back();
        _models.pop_back();
    }

    // remove balls
    while (!_balls.empty())
    {
        delete _balls.back();
        _balls.pop_back();
    }
}
//--------------------------------------------------------------
void App::newFrame()
{
    static int count = 0;
    _frame->writeToFile("vid/img" + ofToString(count++) + ".jpg");

    _frame->endFrame();
    _frame->newFrame();
    _frame->createBrushes();

    stepScene(0.03);

    _timerSet = false;
    _timer = Settings::updateTime;
}
//--------------------------------------------------------------
void App::keyPressed(int key)
{
    switch (key)
    {
        case 'n':
            newFrame();
            break;
    }
}
//--------------------------------------------------------------
void App::keyReleased(int key)
{
}
//--------------------------------------------------------------
void App::mouseMoved(int x, int y )
{
}
//--------------------------------------------------------------
void App::mouseDragged(int x, int y, int button)
{
}
//--------------------------------------------------------------
void App::mousePressed(int x, int y, int button)
{
}
//--------------------------------------------------------------
void App::mouseReleased(int x, int y, int button)
{
}
//--------------------------------------------------------------
void App::windowResized(int w, int h)
{
}
//--------------------------------------------------------------
