#ifndef __MODEL_H__
#define __MODEL_H__

#include "glm/glm.h"

void readOBJ(ofMesh &mesh, istream &in)
{
    std::string line;

    while (!in.eof())
    {
        std::getline(in, line);

        if (line[0] == '#')
            continue;


    }
}

class Model
{
    protected:
        GLMmodel *_model;

    public:
        Model()
        {
            _model = glmReadOBJ("data/dolphin/dolphin_000001.obj");
            glmUnitize(_model);
            glmScale(_model, 30);

            ofMesh
        }

        ~Model()
        {
            glmDelete(_model);
        }

        void draw()
        {
            glPushMatrix();
                glTranslatef(0, 20, 0);
                glmDraw(_model, GLM_SMOOTH);
            glPopMatrix();
        }
};

#endif
