#ifndef __CIELAB_H__
#define __CIELAB_H__

#include "ofGraphics.h"
#include "ofImage.h"

class Palette {
	std::vector<ofColor> colorList;
public:
	Palette(ofImage &img, int numColors);
	ofColor getColor(size_t i) {
		if (i < colorList.size())
			return colorList[i];
		else
			return ofColor::black;
	}
};

struct Cielab {
	float l, a, b;
	Cielab(float L, float A, float B);
	Cielab(const ofColor &rgb);
	ofColor rgb();
	Cielab lerp(const Cielab &r, float f);
};


#endif
