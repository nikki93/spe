#ifndef __COLORXYZ_H__
#define __COLORXYZ_H__

#include <vector>

#include "ofGraphics.h"
#include "ofImage.h"

struct ColorXYZ {
    float v[3]; // the three CIE XYZ components, in order X, Y, Z

    ColorXYZ() { }
    ColorXYZ(float X, float Y, float Z)
    {
        v[0] = X; v[1] = Y; v[2] = Z;
    }
    ColorXYZ(const ofColor &rgb)
    {
        setRgb(rgb.r, rgb.g, rgb.b);
    }

    void setRgb(unsigned char R, unsigned char G, unsigned char B);
    ofColor getRgb();

    ColorXYZ lerp(const ColorXYZ &r, float f);
    float dist(const ColorXYZ &r)
    {
        ofVec3f disp(v[0] - r.v[0], v[1] - r.v[1], v[2] - r.v[2]);
        return disp.length();
    }
};

class Palette {
    typedef std::vector<ColorXYZ> ColorList;
    ColorList _colors;

    public:
        Palette(ofImage &img, int numColors);

        ofColor getColor(size_t i)
        {
            if (i < _colors.size())
                return _colors[i].getRgb();
            else
                return ofColor::black;
        }

        ofColor getClosest(const ofColor &col)
        {
            ColorXYZ xyz(col);

            float minDist = std::numeric_limits<float>::max(), currDist;
            ofColor closest = ofColor::black;

            for (ColorList::iterator i = _colors.begin();
                    i != _colors.end(); ++i)
                if ((currDist = i->dist(xyz)) < minDist)
                {
                    closest = i->getRgb();
                    minDist = currDist;
                }

            return closest;
        }
};

#endif

