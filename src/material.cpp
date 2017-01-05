#include "material.hpp"

Material::Material() :
    materialStr_("default"),
    diffuseCol3f_(0.8f),
    specularCol3f_(1.f),
    specularExp1f_(1.f)
{
    ;
}

Material::~Material()
{
    dfTex_.freeTexture();
    normalTex_.freeTexture();
}

void Material::loadDfTex(const std::string& path)
{
    dfTex_.loadTexture(path);
}

void Material::loadNormalMap(const std::string& path)
{
    normalTex_.loadTexture(path);
}

void Material::bindTextures(const GLenum& df, const GLenum& normal) const
{
    dfTex_.bind(df);
    normalTex_.bind(normal);
}

bool Material::hasDfTex() const
{
    return dfTex_.getID() != 0;
}

bool Material::hasNormalMap() const
{
    return normalTex_.getID() != 0;
}
