#ifndef __MODEL_H__
#define __MODEL_H__

#include "glm/glm.h"
#include "OBJ.h"

class Model
{
    protected:
        GLMmodel *_model;

        ofMesh _mesh[20];
        size_t _frame;
        float _frameTimer;
        float _framePeriod;

    public:
        Model()
            : _frameTimer(2),
            _framePeriod(0.05)
        {
            for (int i = 0; i < 20; ++i)
            {
                char name[50];
                sprintf(name, "data/dolphin/dolphin_%06d.obj", i + 1);
                OBJ::readOBJFromFile(_mesh[i], name,
                        ofMatrix4x4::newScaleMatrix(20, 20, 20));
            }
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
                _frame = (_frame + 1) % 20;
            }
        }

        void draw()
        {
            glPushMatrix(); glTranslatef(0, 20, 0);
                _mesh[_frame].draw();
            glPopMatrix();
        }
};

#endif
