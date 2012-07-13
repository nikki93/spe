#ifndef __COLORXYZ_H__
#define __COLORXYZ_H__

#include <vector>

#include "ofGraphics.h"
#include "ofImage.h"

#include "median_cut.h"

struct ColorXYZ 
{
    float v[3]; // the three CIE XYZ components, in order X, Y, Z

    ColorXYZ() { }
    ColorXYZ(float X, float Y, float Z)
    {
        v[0] = X; v[1] = Y; v[2] = Z;
    }
    ColorXYZ(const ofColor &rgb)
    {
        set(rgb.r, rgb.g, rgb.b);
    }

    operator ofColor() { return getRgb(); }

    void set(unsigned char R, unsigned char G, unsigned char B);
    ofColor getRgb();

    ColorXYZ lerp(const ColorXYZ &r, float f);
};

template<typename Color>
inline float sqDist(const Color &a, const Color &b)
{
    ofVec3f disp(a.v[0] - b.v[0], a.v[1] - b.v[1], a.v[2] - b.v[2]);
    return disp.squareLength();
}

class PaletteBase
{
    public:
        virtual ofColor getClosest(const ofColor &col) = 0;
};

template<typename Color, typename ColorElement>
class Palette : public PaletteBase
{
        typedef std::vector<Color> ColorList;
        ColorList _colors;

    public:
        Palette(ofPixels &img, int paletteSize) 
        {
            // make XYZ image
            int n = img.getWidth() * img.getHeight();

            unsigned char *imgRGB = img.getPixels();
            Color *imgXYZ = new Color[n];

            for (int i = 0; i < n; ++i)
                imgXYZ[i].set(imgRGB[4*i], imgRGB[4*i + 1], imgRGB[4*i + 2]);

            // do the median cut
            _colors = medianCut<Color, ColorElement>(imgXYZ, n, paletteSize);
        }

        ofColor getColor(size_t i)
        {
            if (i < _colors.size())
                return _colors[i].getRgb();
            else
                return ofColor::black;
        }

        ofColor getClosest(const ofColor &col)
        {
            float minDist = std::numeric_limits<float>::max(), currDist;
            Color xyz(col), closest(0, 0, 0);

            for (typename ColorList::iterator i = _colors.begin();
                    i != _colors.end(); ++i)
                if ((currDist = sqDist(*i, xyz)) < minDist)
                {
                    closest = *i;
                    minDist = currDist;
                }

            return closest;
        }
};

#endif

