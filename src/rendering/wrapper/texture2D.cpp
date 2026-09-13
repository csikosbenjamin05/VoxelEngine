//
// Created by beni on 2026. 09. 05..
//

#include "texture2D.h"

#include <iostream>

#include "binaryImage.h"
#include "glad/glad.h"

Texture2D::Texture2D() : ID(0), width(0), height(0), isLoaded(false) {}

Texture2D::~Texture2D() {
    clean();
}

void Texture2D::init(const std::filesystem::path& path, const bool genMipMap) {

    // load and generate the texture
    if (const BinaryImage image(path); image.isValid())
    {
        width = image.width;
        height = image.height;

        // 1. Dynamically determine the correct OpenGL formats
        // 3 color channels, jpgs
        GLenum internalFormat = GL_RGB8;
        GLenum dataFormat = GL_RGB;


        if (image.colorChannels == 4) { // 4 color channels, pngs
            internalFormat = GL_RGBA8;
            dataFormat = GL_RGBA;
        } else if (image.colorChannels == 1) { // 1 color channel, grayscale
            internalFormat = GL_R8;
            dataFormat = GL_RED;
        }

        // 2. Disable 4-byte row alignment to prevent crashes on odd-width images
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        glCreateTextures(GL_TEXTURE_2D, 1, &ID);
        glTextureStorage2D(ID, image.NumberOfMIPLevels(), internalFormat, image.width, image.height);
        glTextureSubImage2D(ID, 0, 0, 0, image.width, image.height, dataFormat, GL_UNSIGNED_BYTE, image.data);

        if (genMipMap)
            glGenerateTextureMipmap(ID);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
        return;
    }

    isLoaded = true;
}

void Texture2D::use(const unsigned int textureUnit) const {
    glBindTextureUnit(textureUnit, ID);
}

void Texture2D::clean() {
    if (isLoaded)
        glDeleteTextures(1, &ID);
    isLoaded = false;
}

Texture2DArray::Texture2DArray() : ID(0), width(0), height(0), layers(0), isLoaded(false) {}
Texture2DArray::~Texture2DArray() { clean(); }

void Texture2DArray::init(const std::filesystem::path& path, const int tileWidth, const int tileHeight, const bool genMipMap) {
    BinaryImage image(path);
    if (!image.isValid()) {
        std::cout << "Failed to load texture" << std::endl;
        return;
    }

    // Convert the 2D atlas strip into 3D array memory
    image.SliceIntoLayers(tileWidth, tileHeight);

    width = image.width;
    height = image.height;
    layers = image.layerCount;

    GLenum internalFormat = (image.colorChannels == 4) ? GL_RGBA8 : GL_RGB8;
    GLenum dataFormat = (image.colorChannels == 4) ? GL_RGBA : GL_RGB;

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // DSA for GL_TEXTURE_2D_ARRAY
    glCreateTextures(GL_TEXTURE_2D_ARRAY, 1, &ID);

    // Allocate 3D storage (Width, Height, Depth/Layers)
    glTextureStorage3D(ID, image.NumberOfMIPLevels(), internalFormat, width, height, layers);

    // Upload the 3D data block
    glTextureSubImage3D(ID, 0, 0, 0, 0, width, height, layers, dataFormat, GL_UNSIGNED_BYTE, image.data);

    if (genMipMap) glGenerateTextureMipmap(ID);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    isLoaded = true;
}

void Texture2DArray::use(const unsigned int textureUnit) const {
    glBindTextureUnit(textureUnit, ID);
}

void Texture2DArray::clean() {
    if (isLoaded) glDeleteTextures(1, &ID);
    isLoaded = false;
}
