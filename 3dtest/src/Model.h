#ifndef __MODEL_H__
#define __MODEL_H__

#include "OBJ.h"

class Model
{
    protected:
        std::vector<ofMesh> _meshes;
        size_t _frame;
        float _frameTimer;
        float _framePeriod;

    public:
        ofVec3f pos;

        // filename of form "... %d ..." if animated for loading frames 1 to numFrames
        // numFrames == 1 means not animated
        Model(const std::string &filename, size_t numFrames = 1, 
                const ofMatrix4x4 &transform = ofMatrix4x4::newIdentityMatrix(),
                float framePeriod = 0.05)
            : _frameTimer(2),
              _framePeriod(framePeriod),
              _meshes(numFrames),
              _frame(0),
              pos(ofVec3f(0, 0, 0))
        {
            if (numFrames > 1)
            {
                char *name = new char[filename.length() + 42];

                for (size_t i = 0; i < numFrames; ++i)
                {
                    sprintf(name, filename.c_str(), i + 1);
                    OBJ::readOBJ(_meshes[i], name, transform);
                }

                delete name;
            }
            else
                OBJ::readOBJ(_meshes[0], filename, transform);
        }

        ~Model()
        {
        }

        void update(float elapsed)
        {
            _frameTimer -= elapsed;
            if (_frameTimer < 0)
            {
                _frameTimer = _framePeriod;
                _frame = (_frame + 1) % _meshes.size();
            }
        }

        void draw()
        {
            ofPushMatrix();
                ofTranslate(pos);
                _meshes[_frame].draw();
            ofPopMatrix();
        }
};

#endif
