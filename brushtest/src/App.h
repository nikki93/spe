#ifndef __APP_H__
#define __APP_H__

#include <ofBaseApp.h>
#include <ofImage.h>

#include "Brush.h"
#include "Field.h"
#include "Cielab.h"

class App : 
    public ofBaseApp
{
    protected:
        Field _forceField;
        ofImage _potential;

        ofImage _color;
		Palette *_palette;

        typedef std::vector<Brush *> BrushList;
        BrushList _brushes;

        ofVec2f _mousePrev;
        float _fieldRedrawTimer;
        bool _fieldRedrawCounting;

    public:
        App() : _forceField(1024, 768) { }

        void setup();
        void update();
        void draw();
        void clear();

        void keyPressed(int key);
        void keyReleased(int key) { }
        void mouseMoved(int x, int y );
        void mouseDragged(int x, int y, int button);
        void mousePressed(int x, int y, int button);
        void mouseReleased(int x, int y, int button);
        void windowResized(int w, int h) { }

        void createBrushes();
};

#endif
