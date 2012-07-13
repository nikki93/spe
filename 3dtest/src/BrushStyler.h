#ifndef __BRUSHSTYLER_H__
#define __BRUSHSTYLER_H__

#include "ofImage.h"
#include "Brush.h"
#include "Field.h"

class BrushStyler {
    protected:
        ofPixels _edges;
        ofPixels _img;
        //Palette _palette;
        Field &_forceField;

        int _iterations;

        int _brushInd; 
        typedef std::vector<Brush *> BrushList; 
        typedef std::vector<BrushList> BrushListList;
        BrushListList _brushes;

        bool containsEdge(int x, int y, float target);

    public:
        BrushStyler(ofPixels &img, ofPixels &edges, Field &field);
        ~BrushStyler() { clear(); }

        void generate();
        void clear();
        void draw();
        bool move();
};

#endif
