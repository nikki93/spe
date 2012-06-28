#include "App.h"

#include <ofGraphics.h>
#include <of3dUtils.h>
#include <ofAppRunner.h>

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
    // 3d!
    glEnable(GL_DEPTH_TEST);
    ofEnableLighting();

    // setup fbos, shaders
    _sceneFBO.allocate(1024, 768);
    _flipShader.load("basicvert.vert", "flip.frag");

    _edgeFBO.allocate(1024, 768, GL_RGB);
    _edgeShader.load("basicvert.vert", "edge.frag");

    _blurXFBO.allocate(1024, 768, GL_RGB);
    _blurXShader.load("basicvert.vert", "blurX.frag");
    _blurXYFBO.allocate(1024, 768, GL_RGB);
    _blurYShader.load("basicvert.vert", "blurY.frag");

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
    // scene --(fixed function)--> _sceneFBO
    _sceneFBO.begin();
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
    _sceneFBO.end();

    // _sceneFBO --(_edgeShader)--> _edgeFBO
    _edgeFBO.begin();
    _edgeShader.begin(); _edgeShader.setUniformTexture("input", _sceneFBO.getTextureReference(), 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        drawQuad(ofVec2f(0, 0), ofVec2f(1024, 768), ofVec2f(1024, 768));
    _edgeShader.end();
    _edgeFBO.end();

    // _sceneFBO --(_blurXShader)--> _blurXFBO
    _blurXFBO.begin();
    _blurXShader.begin(); _blurXShader.setUniformTexture("input", _sceneFBO.getTextureReference(), 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        drawQuad(ofVec2f(0, 0), ofVec2f(1024, 768), ofVec2f(1024, 768));
    _blurXShader.end();
    _blurXFBO.end();

    // _blurXFBO --(_blurYShader)--> _blurXYFBO
    _blurXYFBO.begin();
    _blurYShader.begin(); _blurYShader.setUniformTexture("input", _blurXFBO.getTextureReference(), 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        drawQuad(ofVec2f(0, 0), ofVec2f(1024, 768), ofVec2f(1024, 768));
    _blurYShader.end();
    _blurXYFBO.end();

    // draw FBOs
    _flipShader.begin();
    _edgeFBO.getTextureReference().bind();
    drawQuad(ofVec2f(0, 0), ofVec2f(512, 384), ofVec2f(1024, 768));
    _blurXYFBO.getTextureReference().bind();
    drawQuad(ofVec2f(512, 0), ofVec2f(512, 384), ofVec2f(1024, 768));
    _flipShader.end();
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
