#ifndef TEXTURE_H
#define TEXTURE_H

#include "gl_core_4_1.h"
#include <string>

class Texture {
public:
    Texture();
    ~Texture();

    void loadTexture(const std::string& tPath);
    void freeTexture();

    void bind(const GLenum& textureUnit) const;

    uint32_t getID() const;

protected:
    //Texture id
    uint32_t id_;

    //Texture dimensions
    uint32_t width_;
    uint32_t height_;
};

#endif
