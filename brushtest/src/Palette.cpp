/* color conversion algorithm based on http://www.easyrgb.com/index.php?X=MATH */

#include "Palette.h"
#include "median_cut.h"

Palette::Palette(ofImage &img, int paletteSize) {
    // make XYZ image
    int n = img.getWidth() * img.getHeight();

    unsigned char *imgRGB = img.getPixels();
    ColorXYZ *imgXYZ = new ColorXYZ[n];

    for (int i = 0; i < n; ++i)
        imgXYZ[i].setRgb(imgRGB[i], imgRGB[i + 1], imgRGB[i + 2]);

    // do the median cut
    _colors = medianCut(imgXYZ, n, paletteSize);
}

void ColorXYZ::setRgb(unsigned char R, unsigned char G, unsigned char B) {
    float r = R / 255.0;
    float g = G / 255.0;
    float b = B / 255.0;

    if ( r > 0.04045 ) 
        r = pow( ( r + 0.055 ) / 1.055, 2.4 );
    else 
        r = r / 12.92;
    if ( g > 0.04045 ) 
        g = pow( ( g + 0.055 ) / 1.055, 2.4 );
    else
        g = g / 12.92;
    if ( b > 0.04045 ) 
        b = pow( ( b + 0.055 ) / 1.055, 2.4 );
    else
        b = b / 12.92;

    r *= 100;
    g *= 100;
    b *= 100;

    v[0] = r*0.4124 + g*0.3576 + b*0.1805;
    v[1] = r*0.2126 + g*0.7152 + b*0.0722;
    v[2] = r*0.0193 + g*0.1192 + b*0.9505;
}

ofColor ColorXYZ::getRgb() {
    float r = 0.032406*v[0] + -0.015372*v[1] + -0.004986*v[2];
    float g = -0.009689*v[0] + 0.018758*v[1] + 0.000415*v[2];
    float b = 0.000557*v[0] + -0.002040*v[1] + 0.01057*v[2];

    const float t2 = 1/2.4;

    if ( r > 0.0031308 ) 
        r = 1.055 * ( pow(r, t2) ) - 0.055;
    else
        r = 12.92 * r;
    if ( g > 0.0031308 ) 
        g = 1.055 * ( pow(g, t2) ) - 0.055;
    else
        g = 12.92 * g;
    if ( b > 0.0031308 ) 
        b = 1.055 * ( pow(b, t2) ) - 0.055;
    else
        b = 12.92 * b;

    r *= 255;
    g *= 255;
    b *= 255;

    return ofColor(r, g, b);
}

ColorXYZ ColorXYZ::lerp(const ColorXYZ &r, float f) {
    return ColorXYZ(v[0] + (r.v[0] - v[0]) * f, 
            v[1] + (r.v[1] - v[1]) * f, 
            v[2] + (r.v[2] - v[2]) * f);
}

