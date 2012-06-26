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

    // setup fbo, shader
    _fbo.allocate(1024, 768);
    _shader.load("fbo");

    // camera setup
    _cam.setPosition(ofVec3f(100, 100, 100));
    _cam.lookAt(ofVec3f(0, 0, 0));

    // light setup
    _light.enable();
    _light.setDirectional();
    _light.setDiffuseColor(ofFloatColor(0.8, 0.5, 0.1));

    // make some balls
    int n = 10;
    while (n--)
        _balls.push_back(new Ball(
                    ofVec3f(ofRandom(-100, 100), ofRandom(20, 70), ofRandom(-100, 100)), // pos
                    ofVec3f(0, ofRandom(-5, 5), 0), // vel
                    ofVec3f(0, -20, 0), // acc
                    ofColor(ofRandom(0, 255), ofRandom(0, 255), ofRandom(0, 255)), // col
                    ofRandom(2, 5) // radius
                    ));

    // make the model
    _model = new Model("data/dolphin/dolphin_%06d.obj", 20,
            ofMatrix4x4::newScaleMatrix(20, 20, 20));
    _model->pos = ofVec3f(0, 20, 0);
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

    // model animation
    _model->update(elapsed);
}
//--------------------------------------------------------------
void App::draw()
{
    // render scene to FBO
    _fbo.begin();
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
        ofSetColor(ofColor::gray);
        _model->draw();

        // floor
        ofSetColor(ofColor::white);
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
    _fbo.end();

    // draw FBO
    _shader.begin(); _shader.setUniformTexture("fbo", _fbo.getTextureReference(), 0);

        const int w = 1024, h = 768;
        glBegin(GL_QUADS);  
            glTexCoord2f(0, 0); glVertex3f(0, 0, 0);  
            glTexCoord2f(w, 0); glVertex3f(w, 0, 0);  
            glTexCoord2f(w, h); glVertex3f(w, h, 0);  
            glTexCoord2f(0,h);  glVertex3f(0,h, 0);  
        glEnd();   

    _shader.end();
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
