#ifndef __APP_H__
#define __APP_H__

#include <ofBaseApp.h>
#include <ofImage.h>

#include "Brush.h"

class App : 
    public ofBaseApp
{
    protected:
        Brush *_testBrush;
        ofVec2f _forceField[1024][768];
        ofImage _forcePotential;

        typedef std::vector<Brush *> BrushList;
        BrushList _brushes;

    public:
        void setup();
        void update();
        void draw();
        void clear();

        void keyPressed(int key);
        void keyReleased(int key);
        void mouseMoved(int x, int y );
        void mouseDragged(int x, int y, int button);
        void mousePressed(int x, int y, int button);
        void mouseReleased(int x, int y, int button);
        void windowResized(int w, int h);
};

#endif
