#include "OBJ.h"

#include <algorithm>
#include <cstdio>

namespace OBJ 
{
    // stores all data for a single ofMesh vertex
    struct MeshVertex 
    { 
        ofVec3f v, n; ofVec2f t; 

        bool operator==(const MeshVertex &o)
        {
            return v == o.v && n == o.n && t == o.t;
        }

        MeshVertex &read(const std::string &str, const std::vector<ofVec3f> &vertices,
                const std::vector<ofVec3f> &normals, const std::vector<ofVec2f> &texcoords)
        {
            int vert, norm, tex;
            sscanf(str.c_str(), "%d//%d", &vert, &norm);

            v = vertices[vert - 1];
            n = normals[norm - 1];
            t = ofVec2f(0, 0);

            return *this;
        }
    };

    size_t findOrAdd(std::vector<MeshVertex> &vs, const MeshVertex &v)
    {
        size_t p;
        std::vector<MeshVertex>::iterator i = std::find(vs.begin(), vs.end(), v);
        p = i - vs.begin();
        if (i == vs.end())
            vs.push_back(v);
        return p;
    }

    void readOBJFromFile(ofMesh &mesh, std::string filename, const ofMatrix4x4 &transform)
    {
        std::ifstream in(filename.c_str(), std::ifstream::in);
        if (in.good())
            readOBJFromStream(mesh, in, transform);
        else
            std::cerr << "Error opening file '" << filename 
                << "'" << std::endl;
    }

    void readOBJFromStream(ofMesh &mesh, istream &in, const ofMatrix4x4 &transform)
    {
        std::string line;
        ofVec2f vec2;
        ofVec3f vec3;

        // temporary OBJ lists
        std::vector<ofVec3f> vertices;
        std::vector<ofVec3f> normals;
        std::vector<ofVec2f> texcoords;

        // final list
        std::vector<MeshVertex> buffer;
        MeshVertex vert;

        int i = 0;

        // read the file
        while (!in.eof())
        {
            std::getline(in, line);
            std::stringstream ss(line);

            std::string w;
            ss >> w;

            if (w == "v")
            {
                ss >> vec3.x >> vec3.y >> vec3.z;
                vertices.push_back(vec3 * transform);
            }
            else if (w == "vn")
            {
                ss >> vec3.x >> vec3.y >> vec3.z;
                normals.push_back(vec3);
            }
            else if (w == "vt")
            {
                ss >> vec2.x >> vec2.y;
                texcoords.push_back(vec2);
            }
            else if (w == "f")
            {
                 // find/add vertices
                ss >> w;
                size_t i0 = findOrAdd(buffer, vert.read(w.c_str(), vertices, normals, texcoords));

                ss >> w;
                size_t i1 = findOrAdd(buffer, vert.read(w.c_str(), vertices, normals, texcoords));

                ss >> w;
                size_t i2 = findOrAdd(buffer, vert.read(w.c_str(), vertices, normals, texcoords));

                ss >> w;
                size_t i3 = findOrAdd(buffer, vert.read(w.c_str(), vertices, normals, texcoords));

                // add triangles
                mesh.addTriangle(i0, i1, i2);
                mesh.addTriangle(i0, i2, i3);
            }
        }

        // finally, put the vertex data into the mesh (faces already added)
        for (std::vector<MeshVertex>::iterator i = buffer.begin();
                i != buffer.end(); ++i)
        {
            mesh.addVertex(i->v);
            mesh.addNormal(i->n);
        }
    }
}

