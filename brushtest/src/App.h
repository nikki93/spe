#ifndef __APP_H__
#define __APP_H__
#include <ofBaseApp.h>
#include <ofImage.h>
#include <ofFbo.h>
#include <ofShader.h>
#include "Brush.h"
#include "Field.h"
#include "Palette.h"
#include "BrushStyler.h"
class App : 
    public ofBaseApp
{
    protected:
        Field _forceField;
        ofImage _potential;

        ofImage _color;
        ofImage _edges;

        BrushStyler *_styler;

Palette *_palette;
        
        //BrushList _brushes;
        ofVec2f _mousePrev;
        float _fieldRedrawTimer;
        bool _fieldRedrawCounting;
        ofFbo _fbo;
        ofShader _shader;
        const float *getRadii(int levels, float minRadius, float maxRadius);
       
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
