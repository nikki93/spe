#include "App.h"

#include <ofGraphics.h>
#include <of3dUtils.h>
#include <ofAppRunner.h>

//--------------------------------------------------------------
App::App()
{
}
//--------------------------------------------------------------
void App::setup()
{
    // 3d!
    glEnable(GL_DEPTH_TEST);
    ofEnableLighting();

    // camera setup
    _cam.setPosition(ofVec3f(100, 100, 100));
    _cam.lookAt(ofVec3f(0, 0, 0));

    // light setup
    _light.enable();
    _light.setPosition(ofVec3f(0, 10, 0));
    _light.setPointLight();
    _light.setDiffuseColor(ofFloatColor(0.8, 0.5, 0.1));
    _light.setAttenuation(1, 0.1, 0.1);

    // make some particles
    int n = 10;
    while (n--)
        _balls.push_back(new Ball(
                    ofVec3f(ofRandom(-100, 100), ofRandom(20, 70), ofRandom(-100, 100)), // pos
                    ofVec3f(0, ofRandom(-5, 5), 0), // vel
                    ofVec3f(0, -20, 0), // acc
                    ofColor(ofRandom(0, 255), ofRandom(0, 255), ofRandom(0, 255)), // col
                    ofRandom(2, 5) // radius
                    ));
}
//--------------------------------------------------------------
void App::update()
{
    float elapsed = ofGetLastFrameTime();

    // camera rotation
    ofVec3f camPos = _cam.getPosition();
    camPos.rotate(30 * elapsed, ofVec3f(0, 1, 0));
    _cam.setPosition(camPos);
    _cam.lookAt(ofVec3f(0, 0, 0));

    // balls
    for (std::vector<Ball *>::iterator i = _balls.begin();
            i != _balls.end(); ++i)
        (*i)->update(elapsed);
}
//--------------------------------------------------------------
void App::draw()
{
    _cam.begin();

    // axis
    ofSetColor(ofColor::gray);
    ofSphere(2);
    ofDrawAxis(20);

    // balls
    for (std::vector<Ball *>::iterator i = _balls.begin();
            i != _balls.end(); ++i)
        (*i)->draw();

    // floor
    ofSetColor(ofColor::white);
    glBegin(GL_QUADS);
        glVertex3f(100, 0, 100);
        glVertex3f(100, 0, -100);
        glVertex3f(-100, 0, -100);
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
