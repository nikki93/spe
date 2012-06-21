#ifndef __APP_H__
#define __APP_H__

#include <ofBaseApp.h>
#include <ofCamera.h>
#include <ofLight.h>

#include "Ball.h"

class App : 
    public ofBaseApp
{
    protected:
        ofCamera _cam;

        std::vector<Ball *> _balls;

        ofLight _light;

    public:
        App();

        void setup();
        void update();
        void draw();
        void exit();

        void keyPressed(int key);
        void keyReleased(int key);
        void mouseMoved(int x, int y );
        void mouseDragged(int x, int y, int button);
        void mousePressed(int x, int y, int button);
        void mouseReleased(int x, int y, int button);
        void windowResized(int w, int h);
};

#endif
