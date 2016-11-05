#include "mesh.hpp"

#include "gl_core_4_1.h"

Mesh::Mesh() :
    vao_(0),
    ibo_(0),
    vbo_(0)
{
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
    glGenBuffers(1, &ibo_);
}

Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &vao_);
    glDeleteBuffers(1, &vbo_);
    glDeleteBuffers(1, &ibo_);
}

void Mesh::update(const std::vector<Vertex>& verts, const std::vector<glm::u32vec3>& faces)
{
    glBindVertexArray(vao_);

    // Upload vertex data
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*verts.size(), verts.data(), GL_STATIC_DRAW);

    // Upload face data for indexed draw
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::u32vec3)*faces.size(), faces.data(), GL_STATIC_DRAW);

    // Set vertex attributes
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    // Unbinds for safety, vao first!
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    verts_ = verts;
    faces_ = faces;
}

void Mesh::render() const
{
    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, faces_.size()*3, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
