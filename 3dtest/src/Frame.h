#ifndef __FRAME_H__
#define __FRAME_H__

#include <ofFbo.h>
#include <ofShader.h>

#include "Field.h"
#include "Settings.h"

// handles drawing of a single frames of animation

/*
 * Pipeline
 * --------
 *
 * render: scenedepth, normdepth
 *
 * normdepth --(edge)--> edge
 * scenedepth --(blurX)--> blurX --(blurY)--> blurXY --(grad)--> grad
 *
 * scenedepth, egde, grad --(paint)--> paint
 *
 * paint, edge --(combine)--> result
 *
 */

class Frame
{
    protected:
        Field _field; // field to apply to brushes, changes per-frame

        ofPixels _pix; // temporary space for pixel data

        ofShader _flipShader;

        ofFbo _sceneDepthFBO;
        ofShader _sceneDepthShader;

        ofFbo _normDepthFBO;
        ofShader _normDepthShader;

        ofFbo _edgeFBO;
        ofShader _edgeShader;

        ofFbo _blurXFBO;
        ofShader _blurXShader;
        ofFbo _blurXYFBO;
        ofShader _blurYShader;
        ofFbo _gradFBO;
        ofShader _gradShader;

        ofFbo _paintFBO; // changes per brush update

        ofFbo _combineFBO;
        ofShader _combineShader;
        ofImage _fileImg;

        typedef std::vector<Brush *> BrushList;
        BrushList _brushes;

        App &_app;

    public:
        Frame(App &app)
            : _field(1024, 768),
              _app(app)
        {
            _pix.allocate(1024, 768, OF_IMAGE_COLOR);
            _fileImg.allocate(1024, 768, OF_IMAGE_COLOR);

            // allocate FBOs
            _sceneDepthFBO.allocate(1024, 768, GL_RGBA);
            _normDepthFBO.allocate(1024, 768, GL_RGBA);
            _edgeFBO.allocate(1024, 768, GL_RGBA);
            _blurXFBO.allocate(1024, 768, GL_RGBA);
            _blurXYFBO.allocate(1024, 768, GL_RGBA);
            _gradFBO.allocate(1024, 768, GL_RGBA);
            _paintFBO.allocate(1024, 768, GL_RGBA);
            _combineFBO.allocate(1024, 768, GL_RGB);

            // load shaders
            _flipShader.load("basicvert.vert", "flip.frag");
            _sceneDepthShader.load("scenedepth");
            _normDepthShader.load("normdepth");
            _edgeShader.load("basicvert.vert", "edge.frag");
            _blurXShader.load("basicvert.vert", "blurX.frag");
            _blurYShader.load("basicvert.vert", "blurY.frag");
            _gradShader.load("basicvert.vert", "grad.frag");
            _combineShader.load("basicvert.vert", "combine.frag");
        }

        // start a new animation frame
        void newFrame()
        {
            // first render scene to scene+depth and normals+depth FBOs
            render(_sceneDepthFBO, _sceneDepthShader);
            render(_normDepthFBO, _normDepthShader);

            // normdepth --(edge)--> edge
            pipe(_edgeFBO, _edgeShader, "input", &_normDepthFBO);

            // normdepth --(blur)--(grad)--> grad
            pipe(_blurXFBO, _blurXShader, "input", &_sceneDepthFBO);
            pipe(_blurXYFBO, _blurYShader, "input", &_blurXFBO);
            pipe(_gradFBO, _gradShader, "input", &_blurXYFBO);

            // grad --> field
            _gradFBO.readToPixels(_pix);
            for (int i = 0; i < 1024; ++i)
                for (int j = 0; j < 768; ++j)
                {
                    ofFloatColor c = _pix.getColor(i, j);
                    _field.set(i, j, Settings::fieldMagnitude*ofVec2f(1 - 2*c.g, 2*c.r - 1));
                }

            // clear paint
            _paintFBO.begin();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            ofBackground(ofColor::white);
            _paintFBO.end();
        }

        void combineFrame()
        {
            // edge, paint --(combine)--> combine
            pipe(_combineFBO, _combineShader, "edge", &_edgeFBO, "paint", &_paintFBO);
        }

        // end an animation frame
        void endFrame()
        {
            destroyBrushes();
        }

        // create brushes based on _sceneDepthFBO
        void createBrushes()
        {
            _sceneDepthFBO.readToPixels(_pix);

            for (int i = 0; i < 1024; i += (int) Settings::brushGridStep)
                for (int j = 0; j < 768; j += (int) Settings::brushGridStep)
                {
                    ofColor col = _pix.getColor(i, j);
                    if (col.a < 200)
                        _brushes.push_back(new Brush(ofVec2f(i, j), ofVec2f(0, 0), 
                                    col, Settings::brushRadius, Settings::brushLength, 
                                    Settings::brushDensity, Settings::brushFuzziness));
                }
        }

        // move all brushes based on _field
        void moveBrushes()
        {
            for (BrushList::iterator iter = _brushes.begin();
                    iter != _brushes.end(); )
            {
                Brush *brush = *iter;
                ofVec2f pos = brush->getPosition();
                int i = ofClamp(pos.x, 0, 1023);
                int j = ofClamp(pos.y, 0, 767);

                if (!brush->move(_field.get(i, j), Settings::brushStepTime))
                {
                    delete brush;
                    iter = _brushes.erase(iter);
                }
                else
                    ++iter;
            }
        }

        // draw brushes to _paintFBO
        void drawBrushes()
        {
            _paintFBO.begin();
            glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_ENABLE_BIT);

            glDisable(GL_DEPTH_TEST);
            ofDisableLighting();
            ofEnableAlphaBlending();

            for (BrushList::iterator iter = _brushes.begin();
                    iter != _brushes.end(); ++iter)
                (*iter)->draw();

            glPopAttrib();
            _paintFBO.end();
        }

        // remove all brushes
        void destroyBrushes()
        {
            for (BrushList::iterator iter = _brushes.begin();
                    iter != _brushes.end(); ++iter)
                delete *iter;
            _brushes.clear();
        }

        // write combined result to file
        void writeToFile(const std::string &name)
        {
            _combineFBO.readToPixels(_pix);
            _fileImg.setFromPixels(_pix);
            _fileImg.saveImage(name);
        }

        // helper methods for 'source --(shader)--> dest' stuff
        void render(ofFbo &dest, ofShader &shader)
        {
            dest.begin(); 

            _app.drawScene(shader);

            dest.end();
        }
        void pipe(ofFbo &dest, ofShader &shader, 
                const char *name1 = NULL, ofFbo *input1 = NULL,
                const char *name2 = NULL, ofFbo *input2 = NULL,
                const char *name3 = NULL, ofFbo *input3 = NULL)
        {
            dest.begin();
            shader.begin();

            if (name1)
                shader.setUniformTexture(name1, input1->getTextureReference(), 
                        input1->getTextureReference().getTextureData().textureID);
            if (name2)
                shader.setUniformTexture(name2, input2->getTextureReference(), 
                        input2->getTextureReference().getTextureData().textureID);
            if (name3)
                shader.setUniformTexture(name3, input3->getTextureReference(), 
                        input3->getTextureReference().getTextureData().textureID);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            drawQuad(ofVec2f(0, 0), ofVec2f(1024, 768), ofVec2f(1024, 768));

            shader.end();
            dest.end();
        }

        // helper methods for drawing quads
        static void drawQuad(const ofVec2f &start, const ofVec2f &size, const ofVec2f &texSize)
        {
            glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_ENABLE_BIT);

            glDisable(GL_DEPTH_TEST);
            ofDisableLighting();
            ofDisableAlphaBlending();

            ofVec2f end = start + size;
            glBegin(GL_QUADS);  
            glTexCoord2f(0,         0        ); glVertex3f(start.x, start.y, 0);  
            glTexCoord2f(texSize.x, 0        ); glVertex3f(end.x,   start.y, 0);  
            glTexCoord2f(texSize.x, texSize.y); glVertex3f(end.x,   end.y,   0);  
            glTexCoord2f(0,         texSize.y); glVertex3f(start.x, end.y,   0);  
            glEnd();   

            glPopAttrib();
        }
        static void drawFullScreenQuad()
        {
            drawQuad(ofVec2f(0, 0), ofVec2f(1024, 768), ofVec2f(1024, 768));
        }

        // helper methods to debug FBOs
        void debugDrawFBO(ofFbo &fbo, const ofVec2f &start, const ofVec2f &size)
        {
            _flipShader.begin();

            fbo.getTextureReference().bind();
            drawQuad(start, size, ofVec2f(1024, 768));
            fbo.getTextureReference().unbind();

            _flipShader.end();
        }
        void debugDraw()
        {
            // Available FBOs:
            //     _sceneDepthFBO
            //     _normDepthFBO
            //     _edgeFBO
            //     _blurXFBO
            //     _blurXYFBO
            //     _gradFBO
            //     _paintFBO
            //     _combineFBO

            //_field.draw(ofVec2f(0, 0), 10);

            //debugDrawFBO(_sceneDepthFBO, ofVec2f(0, 0), ofVec2f(512, 384));
            //debugDrawFBO(_edgeFBO, ofVec2f(512, 0), ofVec2f(512, 384));
            //debugDrawFBO(_gradFBO, ofVec2f(0, 384), ofVec2f(512, 384));
            //debugDrawFBO(_combineFBO, ofVec2f(512, 384), ofVec2f(512, 384));

            debugDrawFBO(_combineFBO, ofVec2f(0, 0), ofVec2f(1024, 768));
            //debugDrawFBO(_edgeFBO, ofVec2f(0, 0), ofVec2f(1024, 768));
            //debugDrawFBO(_sceneDepthFBO, ofVec2f(0, 0), ofVec2f(1024, 768));
        }
};

#endif
