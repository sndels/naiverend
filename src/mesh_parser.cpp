#include "mesh_parser.hpp"

#include <glm/glm.hpp>
#include <fstream>
#include <vector>

using glm::vec3;
typedef glm::u32vec3 vec3u;

void parseOBJ(const std::string& obj, Mesh& mesh)
{
    std::ifstream in(obj);
    std::string lineType;
    uint32_t vertCount = 0u;
    uint32_t faceCount = 0u;
    std::vector<Vertex> verts;
    std::vector<vec3u> faces;
    while (in >> lineType) {
        if (lineType == "#") {
            in >> lineType;
            if (lineType == "vertex") {
                in >> lineType;
                in >> lineType;
                in >> vertCount;
            }
            else if (lineType == "face") {
                in >> lineType;
                in >> lineType;
                in >> faceCount;
            }
        }
        else if (lineType == "v") {
            vec3 v;
            in >> v.x;
            in >> v.y;
            in >> v.z;
            verts.push_back(Vertex({v*10.f, vec3(0.0,0.0,0.0)}));
        }
        else if (lineType == "f") {
            vec3u f;
            in >> f.z;
            in >> f.y;
            in >> f.x;
            f -= vec3u(1u,1u,1u);
            faces.push_back(f);
        }
    }
    for (auto& f : faces) {
        vec3 n = cross(verts[f[2]].pos - verts[f[0]].pos, verts[f[1]].pos - verts[f[0]].pos);
        verts[f[0]].normal += n;
        verts[f[1]].normal += n;
        verts[f[2]].normal += n;
    }
    for (auto& v : verts) v.normal = normalize(v.normal);

    mesh.update(verts, faces);
}