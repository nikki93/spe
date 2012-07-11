#include "App.h"
#include <ofGraphics.h>
#include <ofAppRunner.h>
#include <ofImage.h>
#include "Globals.h"
#include "Palette.h"
#include "BrushStyler.h"

#define PALETTE_SIZE 70
#define GRID_STEP 8
#define RADIUS 5
#define DIST 80
#define DENSITY 0.5
#define FUZZINESS 2

void colorTest()
{
    ofColor rgb1 = ofColor::fromHex(0xe20bca);
    ofColor rgb2 = ofColor::fromHex(0x17bb04);
    ColorXYZ xyz1(rgb1);
    ColorXYZ xyz2(rgb2);
    for (int i = 0; i < 1024; ++i)
    {
        float f = i / 1024.0f;
        ofColor tmp(rgb1);
        ofSetColor(tmp.lerp(rgb2, f));
        ofRect(i, 0, i + 1, 384);
        ofSetColor(xyz1.lerp(xyz2, f).getRgb());
        ofRect(i, 384, i + 1, 768);
    }
}
void desaturate(ofPixels &pix) {
    // desaturate
    for (int i = 0; i < 1024; ++i)
        for (int j = 0; j < 768; ++j)
        {
            ofColor col = pix.getColor(i, j);
            pix.setColor(i, j, ofColor(col.getBrightness()));
        }
}
void blur(ofPixels &pix) {
    #define PIX(x, y) pix.getColor(ofClamp(x,0,1023), ofClamp(y,0,767))
    #define RAD 0.2
    // blur X
    for (int x = 0; x < 1024; ++x)
        for (int y = 0; y < 768; ++y)
        {
            ofColor col(0);
            col += PIX(x - 4*RAD, y) * 0.000215825;
            col += PIX(x - 3*RAD, y) * 0.00579007;
            col += PIX(x - 2*RAD, y) * 0.059897;
            col += PIX(x - 1*RAD, y) * 0.241811;
            col += PIX(x        , y) * 0.384572;
            col += PIX(x + 1*RAD, y) * 0.241811;
            col += PIX(x + 2*RAD, y) * 0.059897;
            col += PIX(x + 3*RAD, y) * 0.00579007;
            col += PIX(x + 4*RAD, y) * 0.000215825;
            pix.setColor(x, y, col);
        }
    // blur Y
    for (int x = 0; x < 1024; ++x)
        for (int y = 0; y < 768; ++y)
        {
            ofColor col(0);
            col += PIX(x, y - 4*RAD) * 0.000215825;
            col += PIX(x, y - 3*RAD) * 0.00579007;
            col += PIX(x, y - 2*RAD) * 0.059897;
            col += PIX(x, y - 1*RAD) * 0.241811;
            col += PIX(x, y        ) * 0.384572;
            col += PIX(x, y + 1*RAD) * 0.241811;
            col += PIX(x, y + 2*RAD) * 0.059897;
            col += PIX(x, y + 3*RAD) * 0.00579007;
            col += PIX(x, y + 4*RAD) * 0.000215825;
            pix.setColor(x, y, col);
        }
}
void sobel(Field &field, ofImage &edges, ofPixels &pix) {
    // Sobel gradient
    #define VAL(x, y) pix.getColor(x, y).r
    float mag;
    for (int i = 1; i < 1023; ++i)
        for (int j = 1; j < 767; ++j)
        {
            ofVec2f g(0, 0);
            // left
            g.y -= 2 * VAL(i - 1, j);
            g.y -= VAL(i - 1, j - 1);
            g.y -= VAL(i - 1, j + 1);
            // right
            g.y += 2 * VAL(i + 1, j);
            g.y += VAL(i + 1, j - 1);
            g.y += VAL(i + 1, j + 1);
            // up
            g.x += 2 * VAL(i, j - 1);
            g.x += VAL(i - 1, j - 1);
            g.x += VAL(i + 1, j - 1);
            // down
            g.x -= 2 * VAL(i, j + 1);
            g.x -= VAL(i - 1, j + 1);
            g.x -= VAL(i + 1, j + 1);
            // uniform
            if (g.x < 0)
                g = -g;
            if (g.x == 0)
                g.y = fabs(g.y);
            
            field.set(i, j, 50*g.normalized());
            mag = abs(g.x) + abs(g.y);
            edges.setColor(i, j, ofColor(mag));
        }
}
void flowField(Field &field, ofImage &edges, ofPixels pix)
{
    desaturate(pix);
    blur(pix);
    sobel(field, edges, pix);
}
/*void edgePic(Field &field, ofImage &pix) {
    int mag;
    ofVec2f v;
    for (int i = 0; i < field.width(); i++) {
        for (int j = 0; j < field.height(); j++) {
            v = field.get(i, j);
            mag = abs(v.x) + abs(v.y);
            pix.setColor(i, j, mag);
        }
    }
}*/
//--------------------------------------------------------------
void App::setup()
{
    //_fbo.allocate(1024, 768);
    //_shader.load("canvas");
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofSetBackgroundAuto(false);
    // color image
    _color.loadImage("IMG_6157.png");
    _palette = new Palette(_color, PALETTE_SIZE);
    _edges.allocate(1024, 768, OF_IMAGE_COLOR);
    // force field
    _potential.loadImage("IMG_6157.png");
    flowField(_forceField, _edges, _potential);
    _edges.update();

    _styler = new BrushStyler(_color, _edges, _forceField, *_palette);

    _fieldRedrawTimer = -1;
    _fieldRedrawCounting = false;
}
//--------------------------------------------------------------
void App::update()
{
    float elapsed = ofGetLastFrameTime();
    // count down to field redraw
    if (_fieldRedrawCounting && _fieldRedrawTimer > 0)
        _fieldRedrawTimer -= elapsed;

    _styler->moveBrushes();
    
}
//--------------------------------------------------------------
void App::draw()
{
    
    //_fbo.begin();
    ofSetColor(ofColor::white);
    
    // draw brushes
    _styler->drawBrushes();

    // redraw field if needed
    if (_fieldRedrawCounting && _fieldRedrawTimer <= 0)
        clear();
    
    
    // draw palette
    int hor = sqrt((float) PALETTE_SIZE);
    int n = PALETTE_SIZE;
    for (int i = 0; i < PALETTE_SIZE; ++i)
    {
        int y = i / hor;
        int x = i % hor;
        ofSetColor(_palette->getColor(i));
        ofCircle(x*20 + 10, y*20 + 10, 10);
    }
    
    
    //_fbo.end();
    //_shader.begin();
    //ofSetColor(ofColor::cyan);
    //ofCircle(400, 400, 200);
    //_fbo.draw(0, 0);
    // _shader.end();
}
//--------------------------------------------------------------
void App::clear()
{
    _styler->deleteBrushes();

    // draw background
    ofSetColor(ofColor::white);
    _potential.draw(0, 0);
_edges.draw(0,0);
    _forceField.draw(ofVec2f(0, 0), 10);
    _fieldRedrawCounting = false;
}
//--------------------------------------------------------------
void App::mouseMoved(int x, int y)
{
    _mousePrev.x = x; _mousePrev.y = y;
}
//--------------------------------------------------------------
void App::mouseDragged(int x, int y, int button)
{
    // field brush
    ofVec2f pos(x, y), v;
    ofVec2f disp(pos - _mousePrev);
    disp.normalize();
    disp *= 10;
#define SIZE 20
    int xMin = ofClamp(x - SIZE, 0, 1023),
        xMax = ofClamp(x + SIZE, 0, 1023),
        yMin = ofClamp(y - SIZE, 0, 768),
        yMax = ofClamp(y + SIZE, 0, 768);
    for (v.x = xMin; v.x <= xMax; v.x += 1)
        for (v.y = yMin; v.y <= yMax; v.y += 1)
        {
            float dist = (v - pos).length();
            _forceField.set(v.x, v.y, _forceField.get(v.x, v.y) + disp/dist);
        }
    if (!_fieldRedrawCounting) // redraw soon
    {
        _fieldRedrawCounting = true;
        _fieldRedrawTimer = 0.2;
    }
    _mousePrev.x = x; _mousePrev.y = y;
}
//--------------------------------------------------------------
void App::keyPressed(int key)
{
    switch (key)
    {
        case 'c':
            _forceField.clear();
            break;
        case 'd': // middle
            createBrushes();
            break;
    }
}
//--------------------------------------------------------------
void App::mousePressed(int x, int y, int button)
{
    switch (button)
    {
        case 0: // left
            //_brushes.push_back(tempBrush(ofVec2f(x, y)));
            break;
        case 2: // right
            clear();
            break;
    }
}
//--------------------------------------------------------------
void App::mouseReleased(int x, int y, int button)
{
}
const float * App::getRadii(int levels, float minRadius, float maxRadius) {
    float radiusStep = (maxRadius - minRadius) / (levels - 1);
    float r = minRadius;
    float * sizes = new float[levels];
    for (int i = 0; i < levels; i++) {
        sizes[i] = r;
        r+= radiusStep;
    }
    return sizes;
}
//--------------------------------------------------------------
void App::createBrushes()
{
    _styler->styleBrushes();
    /*ofPixels &pix = _color.getPixelsRef();
    
#define GRID_STEP 12
#define DIST 200
#define DENSITY 0.1
#define FUZZINESS 2
#define RADIUS 6

    // filledness array for brush placement
    bool filled[768][1024];
    for (int y = 0; y < 768; y++)
        for (int x = 0; x < 1024; x++)
            filled[y][x] = false;
    
    int gridStep = 0.8 * _maxRad;
    int levels = 2;
    _maxRad = 30;
    _minRad = 10;
    

    float brushStep;
    if (_maxRad - _minRad == 0)
        brushStep = 1;
    else
        brushStep = 1.0 * (_maxRad - _minRad) / (levels - 1); //0 division!!!!
    _brushes.resize(levels + 2); // + 2 is for the fine layer on top + coarse layer
  
    const float THRESHOLD = 240.0;
    bool cont;
    int lower, upper;
    int ind;
    float currMax;
    float tmp;

    gridStep = 50 * 0.8;
    for (int y = 0; y < 768; y += gridStep) {
        for (int x = 0; x < 1024; x += gridStep) {
            _brushes[0].push_back(new Brush(ofVec2f(x, y), ofVec2f(0, 0), 
                _palette->getClosest(pix.getColor(x, y)), 
                50, DENSITY, 400, FUZZINESS));
        }
    }


    int i;
    // must clean this up..................
    for (i = 1; i < levels; i++) {
        currMax = _maxRad - i * brushStep;
        gridStep = currMax * 0.7;
        for (int y = 0; y < 768; y += gridStep) {
            for (int x = 0; x < 1024; x += gridStep) {
                if (filled[y][x])
                    continue;

                cont = true;
                ind = 0;
                for (int r = 0; r <= currMax + 1; r++) {
                    if (r > _minRad + ind * brushStep)
                        ind++;
                    lower = std::max(0, y - r);
                    upper = std::min(y + r, 768);
                    // left
                    if (x - r >= 0)
                        for (int j = lower; j <= upper; j++)
                            if (_edges.getColor(x - r, j).r >= THRESHOLD) {
                                cont = false;
                                break;
                            }
                    // right
                    if (!cont && x + r <= 1023)
                        for (int j = lower; j <= upper; j++)
                            if (_edges.getColor(x + r, j).r >= THRESHOLD) {
                                cont = false;
                                break;
                            }
                    lower = std::max(0, x - r);
                    upper = std::min(x + r, 1023);
                    // top
                    if (!cont && y - r >= 0)
                        for (int j = lower; j <= upper; j++)
                            if (_edges.getColor(j, y - r).r >= THRESHOLD) {
                                cont = false;
                                break;
                            }
                    // bottom
                    if (!cont && y + r <= 767)
                        for (int j = lower; j <= upper; j++)
                            if (_edges.getColor(j, y + r).r >= THRESHOLD) {
                                cont = false;
                                break;
                            }
                    if (r > currMax) {
                        if (ind > 0) {
                            tmp = _minRad + (ind - 1) * brushStep;
                            _brushes[i].push_back(new Brush(ofVec2f(x, y), ofVec2f(0, 0), 
                                _palette->getClosest(pix.getColor(x, y)), 
                                tmp, DENSITY, DIST, FUZZINESS));
                            for (int m = y - tmp; m <= y + tmp; m++)
                                for (int n = x - tmp; n <= x + tmp; n++)
                                    if (m >= 0 && m <= 767 && n >= 0 && n <= 1023)
                                        filled[m][n] = true;
                        }
                        break;
                    }
                    if (!cont)
                        break;
                }
            }
        }
    }
    
    // fine layer
    for (int y = 0; y < 768; y += 1) {
        for (int x = 0; x < 1024; x += 1) {
            if (_edges.getColor(x, y).r >= THRESHOLD)
                _brushes[i].push_back(new Brush(ofVec2f(x, y), ofVec2f(0, 0), 
                    _palette->getClosest(pix.getColor(x, y)), 1, 1, 100, FUZZINESS));
        }
    }*/
}
//--------------------------------------------------------------

