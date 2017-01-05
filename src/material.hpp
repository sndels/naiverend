#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <glm/glm.hpp>
#include "texture.hpp"

class Material
{
public:
    Material();
    ~Material();
    
    void loadDfTex(const std::string& path);
    void loadNormalMap(const std::string& path);

    // Binds dfTex_ and normalTex_ to df and normal -texture units respectively
    void bindTextures(const GLenum& df, const GLenum& normal) const;
    
    // Checks if material has texture
    bool hasDfTex() const;
    bool hasNormalMap() const;
    
    // Public members for easy access
    std::string materialStr_;
    glm::vec3 diffuseCol3f_;

    glm::vec3 specularCol3f_;
    float specularExp1f_;

private:
    Texture dfTex_;
    Texture normalTex_;
    
};

#endif
