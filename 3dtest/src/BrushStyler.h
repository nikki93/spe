#ifndef __BRUSHSTYLER_H__
#define __BRUSHSTYLER_H__

#include "ofImage.h"
#include "Brush.h"
#include "Palette.h"
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

        bool containsEdge(int x, int y, float threshold, float target);

    public:
        BrushStyler(ofImage &img, ofImage &edges, Field &field);

        void styleBrushes();
        void deleteBrushes();
        void drawBrushes();
        bool moveBrushes();
};

#endif
