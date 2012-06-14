#ifndef __APP_H__
#define __APP_H__

#include <stdio.h>
#include <ofBaseApp.h>
#include <ofTexture.h>
#include <libfreenect.hpp>

class Mutex 
{
    public:
        Mutex() { pthread_mutex_init(&_mutex, NULL); }
        void lock() { pthread_mutex_lock(&_mutex); }
        void unlock() { pthread_mutex_unlock(&_mutex); }

    private:
        pthread_mutex_t _mutex;
};

class KinDevice :
    public Freenect::FreenectDevice
{
    protected:
        std::vector<uint16_t> _depthBuf;
        std::vector<uint8_t> _videoBuf;

        Mutex _depthMutex;
        Mutex _videoMutex;

        bool _newVideoFrame;
        bool _newDepthFrame;

        size_t _videoBufElements;
        size_t _depthBufElements;

    public:
        KinDevice(freenect_context *ctx, int index)
            : Freenect::FreenectDevice(ctx, index),
            _newVideoFrame(false), _newDepthFrame(false)
        {
            _depthBuf.reserve(_depthBufElements =
                    freenect_find_depth_mode(FREENECT_RESOLUTION_MEDIUM, FREENECT_DEPTH_11BIT).bytes/sizeof(uint16_t));
            _videoBuf.reserve(_videoBufElements =
                    freenect_find_video_mode(FREENECT_RESOLUTION_MEDIUM, FREENECT_VIDEO_RGB).bytes/sizeof(uint8_t));
        }

        // data callbacks

        void VideoCallback(void *buf, uint32_t timestamp)
        {
            _videoMutex.lock();

            uint8_t *video = static_cast<uint8_t *>(buf);
            _videoBuf.clear();
            _videoBuf.insert(_videoBuf.begin(), video, video + getVideoBufferElements());
            _newVideoFrame = true;

            _videoMutex.unlock();
        }
        void DepthCallback(void *buf, uint32_t timestamp)
        {
            _depthMutex.lock();

            uint16_t *depth = static_cast<uint16_t *>(buf);
            _depthBuf.clear();
            _depthBuf.insert(_depthBuf.begin(), depth, depth + getDepthBufferElements());
            _newDepthFrame = true;

            _depthMutex.unlock();
        }

        // get data

        size_t getDepthBufferElements()
        {
            return _depthBufElements;
        }
        size_t getVideoBufferElements()
        {
            return _videoBufElements;
        }

        bool newDepthFrame()
        {
            return _newDepthFrame;
        }
        bool newVideoFrame()
        {
            return _newVideoFrame;
        }

        bool getDepth(vector<uint16_t> &buf) 
        {
            bool updated = _newDepthFrame;

            if (_newDepthFrame) 
            {
                _depthMutex.lock();
                buf = _depthBuf;
                _newDepthFrame = false;
                _depthMutex.unlock();
            }

            return updated;
        }
        bool getVideo(vector<uint8_t> &buf) 
        {
            bool updated = _newVideoFrame;

            if (_newVideoFrame) 
            {
                _videoMutex.lock();
                buf = _videoBuf;
                _newVideoFrame = false;
                _videoMutex.unlock();
            }

            return updated;
        }
};

class App : 
    public ofBaseApp
{
    protected:
        KinDevice *_kinDevice;
        Freenect::Freenect *_freenect;

        ofTexture _vidTex;

    public:
        App();

        void setup();
        void update();
        void draw();
        void exit();

        void keyPressed(int key);
        void keyReleased(int key);
        void mouseMoved(int x, int y );
        void mouseDragged(int x, int y, int button);
        void mousePressed(int x, int y, int button);
        void mouseReleased(int x, int y, int button);
        void windowResized(int w, int h);
};

#endif
