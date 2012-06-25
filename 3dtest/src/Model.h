#ifndef __MODEL_H__
#define __MODEL_H__

#include "glm/glm.h"
#include "OBJ.h"

class Model
{
    protected:
        GLMmodel *_model;

        ofMesh _mesh;

    public:
        Model()
        {
            //_model = glmReadOBJ("data/dolphin/dolphin_000001.obj");
            //glmUnitize(_model);
            //glmScale(_model, 30);
            OBJ::readOBJFromFile(_mesh, "data/dolphin/dolphin_000001.obj",
                    ofMatrix4x4::newScaleMatrix(20, 20, 20));
                std::cout << _mesh.getNumVertices();
                std::cout << _mesh.getNumIndices();
        }

        ~Model()
        {
            //glmDelete(_model);
        }

        void draw()
        {
            glPushMatrix(); glTranslatef(0, 20, 0);

                _mesh.draw();

            glPopMatrix();
        }
};

#endif
