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

class Frame;

class App : 
    public ofBaseApp
{
    protected:
        // scene
        ofCamera _cam;
        ofLight _light;
        std::vector<Ball *> _balls;
        Model *_model;

        // rendering
        Frame *_frame;

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
