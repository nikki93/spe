#include "App.h"

#include <ofGraphics.h>
#include <of3dUtils.h>
#include <ofAppRunner.h>

#include "Frame.h"

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
    // graphics setup
    glEnable(GL_DEPTH_TEST);
    ofEnableLighting();

    // renderer setup
    _frame = new Frame(*this);

    // camera
    _cam.setPosition(ofVec3f(100, 100, 100));
    _cam.lookAt(ofVec3f(0, 0, 0));

    // light
    _light.enable();
    _light.setPointLight();
    _light.setDiffuseColor(ofFloatColor(0.8, 0.5, 0.1));
    _light.setPosition(40, 40, 40);

    // balls
    int n = 10;
    while (n--)
        _balls.push_back(new Ball(
                    ofVec3f(ofRandom(-100, 100), ofRandom(20, 70), ofRandom(-100, 100)), // pos
                    ofVec3f(0, ofRandom(-5, 5), 0), // vel
                    ofVec3f(0, -20, 0), // acc
                    ofColor(ofRandom(0, 255), ofRandom(0, 255), ofRandom(0, 255)), // col
                    ofRandom(2, 5) // radius
                    ));

    // model
    _model = new Model("data/dolphin/dolphin_%06d.obj", 20,
            ofMatrix4x4::newScaleMatrix(20, 20, 20));
    _model->pos = ofVec3f(0, 20, 0);

    // render first frame
    _frame->newFrame();
    _frame->createBrushes();
}
//--------------------------------------------------------------
void App::update()
{
    //float elapsed = ofGetLastFrameTime();
    float elapsed = 0.03;

    // camera rotation
    ofVec3f camPos = _cam.getPosition();
    camPos.rotate(30 * elapsed, ofVec3f(0, 1, 0));
    _cam.setPosition(camPos);
    _cam.lookAt(ofVec3f(0, 0, 0));

    // balls
    for (std::vector<Ball *>::iterator i = _balls.begin();
            i != _balls.end(); ++i)
        (*i)->update(elapsed);

    // model animation
    _model->update(elapsed);

    // rendering
    _frame->moveBrushes();
    _frame->drawBrushes();
    _frame->combineFrame();
}
//--------------------------------------------------------------
void App::draw()
{
    _frame->debugDraw();
}
//--------------------------------------------------------------
void App::drawScene()
{
    _cam.begin();

        // clear
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // axis
        ofSetColor(ofColor::gray);
        ofSphere(2);
        ofDrawAxis(20);

        // balls
        for (std::vector<Ball *>::iterator i = _balls.begin();
                i != _balls.end(); ++i)
            (*i)->draw();

        // model
        ofSetColor(ofColor::fromHex(0x6088d7));
        _model->draw();

        // floor
        ofSetColor(ofColor::fromHex(0xd8baba));
        glBegin(GL_QUADS);
            glNormal3f(0, 1, 0);
            glVertex3f(100, 0, 100);
            glNormal3f(0, 1, 0);
            glVertex3f(100, 0, -100);
            glNormal3f(0, 1, 0);
            glVertex3f(-100, 0, -100);
            glNormal3f(0, 1, 0);
            glVertex3f(-100, 0, 100);
        glEnd();

    _cam.end();
}
//--------------------------------------------------------------
void App::exit()
{
    // remove balls
    while (!_balls.empty())
    {
        delete _balls.back();
        _balls.pop_back();
    }

    // remove model
    delete _model;
}
//--------------------------------------------------------------
void App::keyPressed(int key)
{
    switch (key)
    {
        case 'n':
            _frame->endFrame();
            _frame->newFrame();
            _frame->createBrushes();
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
