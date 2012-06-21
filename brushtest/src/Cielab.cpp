/* color conversion algorithm based on http://www.easyrgb.com/index.php?X=MATH */

#include "Cielab.h"
#include "median_cut.h"

Palette::Palette (ofImage &img, int numColors) {
	ofColor tmp;

	std::list<Point> palette = medianCut((Point *) img.getPixels(), img.getWidth() * img.getHeight(), numColors);
	std::list<Point>::iterator iter;
	for (iter = palette.begin() ; iter != palette.end(); iter++) {
		tmp = ofColor((int)iter->x[0], (int)iter->x[1], (int)iter->x[2]);
		colorList.push_back(tmp);	
    }
}

Cielab::Cielab(float L, float A, float B)
	: l(L), a(A), b(B) {
}

Cielab::Cielab(const ofColor &rgb) {
	float r = rgb.r / 255.0;
	float g = rgb.g / 255.0;
	float b = rgb.b / 255.0;

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

	float x = r*0.4124 + g*0.3576 + b*0.1805;
	float y = r*0.2126 + g*0.7152 + b*0.0722;
	float z = r*0.0193 + g*0.1192 + b*0.9505;
	
	x /= 95.047;
	y /= 100;
	z /= 108.883;

	//float x = 0.43389*r + 0.39728*g + 0.18991*b;
	//float y = 0.2126*r + 0.7152*g + 0.0722*b;
	//float z = 0.017725*r + 0.10948*g + 0.87296*b;
	

	const float t1 = 1/3.0, t2 = 16/116.0;
	if ( x > 0.008856 ) 
		x = pow(x, t1);
	else                    
		x = ( 7.787 * x ) + t2;
	if ( y > 0.008856 ) 
		y = pow(y, t1);
	else                    
		y = ( 7.787 * y ) + t2;
	if ( z > 0.008856 ) 
		z = pow(z, t1);
	else                    
		z = ( 7.787 * z ) + t2;

	l = 116*y - 16;
	a = 500*(x-y);
	this->b = 200*(y-z);
}

ofColor Cielab::rgb() {
	float y = (l + 16) / 116;
	float x = a/500 + y;
	float z = y - b/200;
	float t1, t2 = 16 / 116.0;

	/*if ( (t1 = pow(y, 3)) > 0.008856 ) 
		y = t1;
	else
		y = ( y - t2 ) / 7.787;
	if ( (t1 = pow(x, 3)) > 0.008856 )
		x = t1 * 0.95047;
	else
		x = ( x - t2 ) * 0.12206;
	if ( (t1 = pow(z, 3)) > 0.008856 ) 
		z = t1 * 1.08883;
	else
		z = ( z - t2 ) * 0.13983;*/

	if ( (t1 = pow(y, 3)) > 0.008856 ) 
		y = t1;
	else
		y = ( y - t2 ) / 7.787;
	if ( (t1 = pow(x, 3)) > 0.008856 )
		x = t1;
	else
		x = ( x - t2 ) / 7.787;
	if ( (t1 = pow(z, 3)) > 0.008856 ) 
		z = t1;
	else
		z = ( z - t2 ) / 7.787;

	x *= 0.95047;
	z *= 1.08883;

	float r = 3.2406*x + -1.5372*y + -0.4986*z;
	float g = -0.9689*x + 1.8758*y + 0.0415*z;
	float b = 0.0557*x + -0.2040*y + 1.057*z;

	t2 = 1/2.4;
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

Cielab Cielab::lerp(const Cielab &r, float f) {
	return Cielab(l + (r.l - l) * f, 
		a + (r.a - a) * f, 
		b + (r.b - b) * f);
}
