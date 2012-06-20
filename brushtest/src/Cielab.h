#ifndef __COLORS_H__
#define __COLORS_H__

#include "ofGraphics.h"
#include "ofImage.h"

class Palette {
	vector<ofColor *> colorlist;
	
public:
	Palette(ofImage img);
};

struct Cielab {
	float l, a, b;
	Cielab(float L, float A, float B);
	Cielab(const ofColor &rgb);
	ofColor rgb();
	Cielab operator+(const Cielab &r);
	Cielab operator-(const Cielab &r);
	Cielab operator*(float f);
	Cielab lerp(const Cielab &r, float f);
};


#endif
