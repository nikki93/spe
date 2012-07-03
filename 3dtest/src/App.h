#ifndef __APP_H__
#define __APP_H__

#include <ofBaseApp.h>
#include <ofCamera.h>
#include <ofLight.h>
#include <ofFbo.h>
#include <ofShader.h>
#include <ofImage.h>

#include "Ball.h"
#include "Model.h"
#include "Brush.h"
#include "Field.h"

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

class App : 
    public ofBaseApp
{
    protected:
        ofCamera _cam;
        ofLight _light;
        std::vector<Ball *> _balls;
        Model *_model;
        Field _field;
        ofPixels _pix;
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

        ofFbo _paintFBO;

        ofFbo _combineFBO;
        ofShader _combineShader;

        typedef std::vector<Brush *> BrushList;
        BrushList _brushes;

        ofImage _screenshot;

    public:
        App();

        void setup();
        void update();
        void draw();
        void exit();

        void drawScene();

        void keyPressed(int key);
        void keyReleased(int key);
        void mouseMoved(int x, int y );
        void mouseDragged(int x, int y, int button);
        void mousePressed(int x, int y, int button);
        void mouseReleased(int x, int y, int button);
        void windowResized(int w, int h);
};

#endif
