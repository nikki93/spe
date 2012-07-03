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
    : _field(1024, 768)
{
}
//--------------------------------------------------------------
void App::setup()
{
    // graphics setup
    glEnable(GL_DEPTH_TEST);
    ofEnableLighting();
    _screenshot.allocate(1024, 768, OF_IMAGE_COLOR);
    _pix.allocate(1024, 768, OF_IMAGE_COLOR);
    //ofSetBackgroundAuto(false);

    // setup fbos, shaders
    _flipShader.load("basicvert.vert", "flip.frag");

    _sceneDepthFBO.allocate(1024, 768, GL_RGBA);
    _sceneDepthShader.load("scenedepth");

    _normDepthFBO.allocate(1024, 768, GL_RGBA);
    _normDepthShader.load("normdepth");

    _edgeFBO.allocate(1024, 768, GL_RGBA);
    _edgeShader.load("basicvert.vert", "edge.frag");

    _blurXFBO.allocate(1024, 768, GL_RGBA);
    _blurXShader.load("basicvert.vert", "blurX.frag");
    _blurXYFBO.allocate(1024, 768, GL_RGBA);
    _blurYShader.load("basicvert.vert", "blurY.frag");

    _gradFBO.allocate(1024, 768, GL_RGBA);
    _gradShader.load("basicvert.vert", "grad.frag");

    _paintFBO.allocate(1024, 768, GL_RGBA);
    _combineFBO.allocate(1024, 768, GL_RGB);
    _combineShader.load("basicvert.vert", "combine.frag");

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

    // render --(_sceneDepthShader)--> _sceneDepthFBO
    _sceneDepthFBO.begin();
    _sceneDepthShader.begin();
        drawScene();
    _sceneDepthShader.end();
    _sceneDepthFBO.end();

    // scene --(_normDepthShader)--> _normDepthFBO
    _normDepthFBO.begin();
    _normDepthShader.begin();
      drawScene();
    _normDepthShader.end();
    _normDepthFBO.end();

    // _normDepthFBO --(_edgeShader)--> _edgeFBO
    _edgeFBO.begin();
    _edgeShader.begin(); _edgeShader.setUniformTexture("input", _normDepthFBO.getTextureReference(), 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        drawQuad(ofVec2f(0, 0), ofVec2f(1024, 768), ofVec2f(1024, 768));
    _edgeShader.end();
    _edgeFBO.end();

    // _normDepthFBO --(_blurXShader)--> _blurXFBO --(_blurYShader)--> _blurXYFBO --(_gradShader) --> _gradFBO
    _blurXFBO.begin();
    _blurXShader.begin(); _blurXShader.setUniformTexture("input", _sceneDepthFBO.getTextureReference(), 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        drawQuad(ofVec2f(0, 0), ofVec2f(1024, 768), ofVec2f(1024, 768));
    _blurXShader.end();
    _blurXFBO.end();

    _blurXYFBO.begin();
    _blurYShader.begin(); _blurYShader.setUniformTexture("input", _blurXFBO.getTextureReference(), 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        drawQuad(ofVec2f(0, 0), ofVec2f(1024, 768), ofVec2f(1024, 768));
    _blurYShader.end();
    _blurXYFBO.end();

    _gradFBO.begin();
    _gradShader.begin(); _gradShader.setUniformTexture("input", _blurXYFBO.getTextureReference(), 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        drawQuad(ofVec2f(0, 0), ofVec2f(1024, 768), ofVec2f(1024, 768));
    _gradShader.end();
    _gradFBO.end();

    // draw FBOs
    _flipShader.begin();

        _sceneDepthFBO.getTextureReference().bind();
        drawQuad(ofVec2f(0, 0), ofVec2f(512, 384), ofVec2f(1024, 768));

        _normDepthFBO.getTextureReference().bind();
        drawQuad(ofVec2f(512, 0), ofVec2f(512, 384), ofVec2f(1024, 768));

        _edgeFBO.getTextureReference().bind();
        drawQuad(ofVec2f(0, 0), ofVec2f(1024, 768), ofVec2f(1024, 768));

        _gradFBO.getTextureReference().bind();
        drawQuad(ofVec2f(512, 384), ofVec2f(512, 384), ofVec2f(1024, 768));

    _flipShader.end();

    // field
    _gradFBO.readToPixels(_pix);
    for (int i = 0; i < 1024; ++i)
        for (int j = 0; j < 768; ++j)
        {
            ofFloatColor c = _pix.getColor(i, 768 - j);
            _field.set(i, j, ofVec2f(1 - 2*c.g, 2*c.r - 1));
        }

    //_field.draw(ofVec2f(0, 0), 6);
    //return;

    // brushes
#define GRID_STEP 10
#define RADIUS 8
#define DIST 100
#define DENSITY 0.5
#define FUZZINESS 2

    _paintFBO.begin();
        _sceneDepthFBO.readToPixels(_pix);
        for (int i = 0; i < 1024; i += GRID_STEP)
            for (int j = 0; j < 768; j += GRID_STEP)
            {
                ofColor col = _pix.getColor(i, 768 - j);
                _brushes.push_back(new Brush(ofVec2f(i, j), ofVec2f(0, 0), 
                            col, RADIUS, DIST, DENSITY, 
                            FUZZINESS));
            }

        glDisable(GL_DEPTH_TEST);
        ofEnableAlphaBlending();
        int N = 20;
        while (N--)
        {
            for (BrushList::iterator iter = _brushes.begin();
                    iter != _brushes.end(); )
            {
                Brush *brush = *iter;
                brush->draw();
                ofVec2f pos = brush->getPosition();
                int i = ofClamp(pos.x, 0, 1023);
                int j = ofClamp(pos.y, 0, 767);

                if (!brush->move(500*_field.get(i, j), 0.05))
                {
                    delete brush;
                    iter = _brushes.erase(iter);
                }
                else
                    ++iter;
            }
            for (BrushList::iterator iter = _brushes.begin();
                    iter != _brushes.end(); ++iter)
                (*iter)->draw();
        }
        glEnable(GL_DEPTH_TEST);
        ofDisableAlphaBlending();

        for (BrushList::iterator iter = _brushes.begin();
                iter != _brushes.end(); ++iter)
            delete *iter;
        _brushes.clear();
    _paintFBO.end();

    // final combine
    _combineFBO.begin();
    _combineShader.begin(); 
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        _combineShader.setUniformTexture("edge", _edgeFBO.getTextureReference(), _edgeFBO.getTextureReference().getTextureData().textureID);
        _combineShader.setUniformTexture("paint", _paintFBO.getTextureReference(), _paintFBO.getTextureReference().getTextureData().textureID);
        drawQuad(ofVec2f(0, 0), ofVec2f(1024, 768), ofVec2f(1024, 768));
    _combineShader.end();
    _combineFBO.end();

    // screenshot
    static int counter = 0;

    _combineFBO.readToPixels(_pix);
    _screenshot.setFromPixels(_pix);

    _screenshot.saveImage("vid/img" + ofToString(counter) + ".jpg");
    ++counter;
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
void App::draw()
{
    _combineFBO.draw(0, 0);
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
