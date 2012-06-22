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

inline float dist(const ofColor &a, const ofColor &b)
{
    ofVec3f disp(a.r - b.r, a.g - b.g, a.b - b.b);
    return disp.length();
}

class Palette {
    typedef std::vector<ofColor> ColorList;
    ColorList _colors;

    public:
        Palette(ofImage &img, int numColors);

        ofColor getColor(size_t i)
        {
            if (i < _colors.size())
                return _colors[i];
            else
                return ofColor::black;
        }

        /*
        ofColor getClosest(const ofColor &col)
        {
            ColorXYZ xyz(col);

            float minDist = std::numeric_limits<float>::max(), currDist;
            ColorXYZ closest(0, 0, 0);

            for (ColorList::iterator i = _colors.begin();
                    i != _colors.end(); ++i)
                if ((currDist = i->dist(xyz)) < minDist)
                {
                    closest = *i;
                    minDist = currDist;
                }

            return closest.getRgb();
        }
        */
        ofColor getClosest(const ofColor &col)
        {
            float minDist = std::numeric_limits<float>::max(), currDist;
            ofColor closest(0, 0, 0);

            for (ColorList::iterator i = _colors.begin();
                    i != _colors.end(); ++i)
                if ((currDist = dist(*i, col)) < minDist)
                {
                    closest = *i;
                    minDist = currDist;
                }

            return closest;
        }
};

#endif

