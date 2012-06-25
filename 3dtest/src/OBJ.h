#ifndef __OBJ_H__
#define __OBJ_H__

#include <ofMesh.h>
#include <ofMatrix4x4.h>

namespace OBJ
{
    void readOBJFromFile(ofMesh &mesh, std::string filename, 
            const ofMatrix4x4 &transform = ofMatrix4x4::newIdentityMatrix());
    void readOBJFromStream(ofMesh &mesh, istream &in,
            const ofMatrix4x4 &transform = ofMatrix4x4::newIdentityMatrix());
}

#endif
