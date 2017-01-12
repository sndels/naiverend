#include "model.hpp"

#include "gl_core_4_1.h"

Model::Model() :
    vao_(0),
    ibo_(0),
    vbo_(0)
{
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
    glGenBuffers(1, &ibo_);
}

Model::~Model()
{
    glDeleteVertexArrays(1, &vao_);
    glDeleteBuffers(1, &vbo_);
    glDeleteBuffers(1, &ibo_);
}

void Model::update(const std::vector<Vertex>& verts, const std::vector<glm::u32vec3>& faces, std::shared_ptr<Material> mat)
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
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texture));
    

    // Unbinds for safety, vao first!
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    verts_ = std::move(verts);
    faces_ = std::move(faces);
    mat_ = mat;
}

void Model::render(BasicProgram& sp) const
{
    sp.updateHasDiffuseTex(mat_->hasDfTex());
    sp.updateHasNormalMap(mat_->hasNormalMap());
    sp.updateDiffuseCol(mat_->diffuseCol3f_);
    sp.updateSpecularCol(mat_->specularCol3f_);
    sp.updateSpecularExp(mat_->specularExp1f_);

    glBindVertexArray(vao_);

    mat_->bindTextures(GL_TEXTURE0, GL_TEXTURE1);
    glDrawElements(GL_TRIANGLES, faces_.size()*3, GL_UNSIGNED_INT, 0);
    
    glBindVertexArray(0);
}
