#include "texture.hpp"

#include <iostream>
#include <sstream>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "../lib/stb_image.h"

using std::endl;

Texture::Texture() :
    id_(0),
    width_(0),
    height_(0)
{
    ;
}


Texture::~Texture()
{
    freeTexture();
}

void Texture::freeTexture()
{
    width_ = 0;
    height_ = 0;
    glDeleteTextures(1, &id_);
}

void Texture::loadTexture(const std::string& tPath)
{
    freeTexture();

    int32_t w, h;
    int channels;
    uint8_t* pixels = stbi_load(tPath.c_str(), &w, &h, &channels, 4);
    width_ = w;
    height_ = h;
    if(pixels == NULL) {
        std::stringstream err;
        err << "Error loading texture " << tPath << endl;
        err << "Error loading pixel data from file!" << endl;
        throw std::runtime_error(err.str());
    }
    
    //Get a new id and bind it to texture
    glGenTextures(1, &id_);
    glBindTexture(GL_TEXTURE_2D, id_);

    //Generate texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width_, height_, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, (const GLvoid*)pixels);

    //Set scaling to linear
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    //Unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(pixels);

    //Check for error
    GLenum error = glGetError();
    if(error != GL_NO_ERROR) {
        std::stringstream err;
        err << "Error loading texture " << tPath << ':' << endl;
        err << "Error loading texture from pixel data!" << endl;
        throw std::runtime_error(err.str());
    }
}

void Texture::bind(const GLenum& textureUnit) const
{
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_2D, id_);
}

GLuint Texture::getID() const
{
    return id_;
}
