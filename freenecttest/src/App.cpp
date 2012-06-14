#include "App.h"

#include <cstdio>
#include <ofGraphics.h>
#include <libfreenect.hpp>

void blur(std::vector<uint16_t> &depth, int w, int h)
{
    const int halfN = 3;
    //static float coeff[5] = { 0.00112237,  0.152931,  0.691892,  0.152931, 0.00112237 };
    static float coeff[] = { 0.000817774, 0.0286433,  0.235018,  0.471041,  0.235018, 0.0286433, 0.000817774 };
    std::vector<uint16_t> buf(depth.size());

#define DEPTH(x, y) depth[(y)*w + x]
#define BUF(x, y) buf[(y)*w + x]

    // x pass
    for (int x = 0; x < w; ++x)
        for (int y = 0; y < h; ++y)
        {
            uint16_t p = 0;
            for (int dx = std::max(-halfN, -x); dx < std::min(halfN + 1, w - x); ++dx)
                p += coeff[dx + halfN] * DEPTH(x + dx, y);
            BUF(x, y) = p;
        }

    // y pass
    for (int x = 0; x < w; ++x)
        for (int y = 0; y < h; ++y)
        {
            uint16_t p = 0;
            for (int dy = std::max(-halfN, -x); dy < std::min(halfN + 1, h - y); ++dy)
                p += coeff[dy + halfN] * BUF(x, y + dy);
            DEPTH(x, y) = p;
        }
}

//--------------------------------------------------------------
App::App()
{
}
//--------------------------------------------------------------
void App::setup()
{
    // setup Kinect
    _freenect = new Freenect::Freenect();
    _kinDevice = &(_freenect->createDevice<KinDevice>(0));
    _kinDevice->setTiltDegrees(10);
    _kinDevice->startVideo();
    _kinDevice->startDepth();
    _kinDevice->setLed(LED_GREEN);

    // setup video texture
    _vidTex.allocate(640, 480, GL_LUMINANCE);
}
//--------------------------------------------------------------
void App::update()
{
    const int w = 640;
    const int h = 480;

    if (_kinDevice->newVideoFrame() && _kinDevice->newDepthFrame())
    {
        std::vector<uint16_t> depthBuf;
        _kinDevice->getDepth(depthBuf);

        blur(depthBuf, w, h);

#define PIXEL(i, j) (pixels[(j)*w + i])
#define DEPTH(i, j) (depthBuf[(j)*w + i])
        uint8_t *pixels = new uint8_t[w*h];
        for (int x = 0; x < w; ++x)
            for (int y = 0; y < h; ++y)
            {
                /*
                float f = -4.0*DEPTH(x, y);

                if (x > 0)
                    f += DEPTH(x - 1, y);
                if (x < w - 1)
                    f += DEPTH(x + 1, y);
                if (y > 1)
                    f += DEPTH(x, y - 1);
                if (y < h - 1)
                    f += DEPTH(x, y + 1);

                PIXEL(x, y) = fabs(f)/2048.0 * 255;
                */

                PIXEL(x, y) = DEPTH(x, y)/2048.0 * 255;
            }

        _vidTex.loadData(pixels, 640, 480, GL_LUMINANCE);
    }
}
//--------------------------------------------------------------
void App::draw()
{
    _vidTex.draw(0, 0);
}
//--------------------------------------------------------------
void App::exit()
{
    // stop Kinect
    _kinDevice->stopDepth();
    _kinDevice->stopVideo();
    _kinDevice->setLed(LED_OFF);
    delete _freenect;
}
//--------------------------------------------------------------
void App::keyPressed(int key)
{
}
//--------------------------------------------------------------
void App::keyReleased(int key)
{
}
//--------------------------------------------------------------
void App::mouseMoved(int x, int y )
{
}
//--------------------------------------------------------------
void App::mouseDragged(int x, int y, int button)
{
}
//--------------------------------------------------------------
void App::mousePressed(int x, int y, int button)
{
}
//--------------------------------------------------------------
void App::mouseReleased(int x, int y, int button)
{
}
//--------------------------------------------------------------
void App::windowResized(int w, int h)
{
}
//--------------------------------------------------------------
