#ifndef MESH_H
#define MESH_H

#include <glm/glm.hpp>
#include <vector>

struct Vertex
{
   glm::vec3 pos;
   glm::vec3 normal;
};

class Mesh
{
public:
    Mesh();
    ~Mesh();

    void update(const std::vector<Vertex>& verts, const std::vector<glm::u32vec3>& faces);
    void render() const;

private:
    std::vector<Vertex> verts_;
    std::vector<glm::u32vec3> faces_;

    uint32_t vao_;
    uint32_t ibo_;
    uint32_t vbo_;

};

#endif
