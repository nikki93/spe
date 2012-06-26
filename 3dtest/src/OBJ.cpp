#include "OBJ.h"

#include <algorithm>
#include <cstdio>
#include <map>

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

        MeshVertex()
        {
        }

        MeshVertex(ofVec3f V, ofVec3f N, ofVec2f T)
            : v(V), n(N), t(T)
        {
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
    typedef std::map<MeshVertex, size_t> IndexMap;

    // lexicographical ordering on all POD elements
    bool operator<(const MeshVertex &a, const MeshVertex &b)
    {
        if (a.v.x < b.v.x)
            return true;
        else if (a.v.x == b.v.x)
            if (a.v.y < b.v.y)
                return true;
            else if (a.v.y == b.v.y)
                if (a.v.z < b.v.z)
                    return true;
                else if (a.v.z == b.v.z)
                    if (a.n.x < b.n.x)
                        return true;
                    else if (a.n.x == b.n.x)
                        if (a.n.y < b.n.y)
                            return true;
                        else if (a.n.y == b.n.y)
                            if (a.n.z < b.n.z)
                                return true;
                            else if (a.n.z == b.n.z)
                                if (a.t.x < b.t.x)
                                    return true;
                                else if (a.t.x == b.t.x)
                                    return a.t.y < b.t.y;
        return false;
    }

    void readOBJ(ofMesh &mesh, std::string filename, const ofMatrix4x4 &transform)
    {
        std::ifstream in(filename.c_str(), std::ifstream::in);
        if (in.good())
            readOBJ(mesh, in, transform);
        else
            std::cerr << "Error opening file '" << filename 
                << "'" << std::endl;
    }

    void readOBJ(ofMesh &mesh, istream &in, const ofMatrix4x4 &transform)
    {
        std::string line;
        ofVec2f vec2;
        ofVec3f vec3;

        // temporary OBJ lists
        std::vector<ofVec3f> vertices;
        std::vector<ofVec3f> normals;
        std::vector<ofVec2f> texcoords;

        // final list
        IndexMap indices;
        MeshVertex vert;

        size_t i = 0; // index counter

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
                // make triangles from first vertex and every adjacent
                // pair among other ones

                ss >> w;
                vert.read(w.c_str(), vertices, normals, texcoords);
                std::pair<IndexMap::iterator, bool> ret
                    = indices.insert(std::make_pair(vert, i));
                if (ret.second)
                    ++i;
                size_t i0 = ret.first->second; // first

                ss >> w;
                vert.read(w.c_str(), vertices, normals, texcoords);
                ret = indices.insert(std::make_pair(vert, i));
                if (ret.second)
                    ++i;
                size_t i1 = ret.first->second; // pair vertex 1

                while (!ss.eof())
                {
                    ss >> w;
                    vert.read(w.c_str(), vertices, normals, texcoords);
                    ret = indices.insert(std::make_pair(vert, i));
                    if (ret.second)
                        ++i;
                    size_t i2 = ret.first->second; // pair vertex 2

                    mesh.addTriangle(i0, i1, i2); // add triangle

                    i1 = i2; // push pair vertex 2 to pair vertex 1
                }
            }
        }

        // finally, put the vertex data into the mesh (faces already added)
        int n = indices.size();

        ofVec3f *verts = new ofVec3f[n];
        ofVec3f *norms = new ofVec3f[n];
        ofVec2f *texs = new ofVec2f[n];

        for (IndexMap::iterator i = indices.begin();
                i != indices.end(); ++i)
        {
            verts[i->second] = i->first.v;
            norms[i->second] = i->first.n;
            texs[i->second] = i->first.t;
        }

        mesh.addVertices(verts, n);
        mesh.addNormals(norms, n);
        mesh.addTexCoords(texs, n);
    }
}

