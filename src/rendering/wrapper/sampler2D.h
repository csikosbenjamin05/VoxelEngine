//
// Created by beni on 2026. 09. 05..
//
#pragma once

#include <glad/glad.h>
#include <glm/vec3.hpp>

struct Sampler2D {
    enum class WrapMode : GLenum {
        REPEAT          = GL_REPEAT,
        MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
        CLAMP_TO_EDGE   = GL_CLAMP_TO_EDGE,
        CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER
    };

    enum class FilterMode : GLenum {
        NEAREST = GL_NEAREST,
        LINEAR = GL_LINEAR,
        NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
        LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
        NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
        LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR
    };

    Sampler2D();
    ~Sampler2D();

    void init(WrapMode wrap_S = WrapMode::REPEAT, WrapMode wrap_T = WrapMode::REPEAT,
        FilterMode magnifyingFilter = FilterMode::LINEAR,
        FilterMode minifyingFilter = FilterMode::LINEAR_MIPMAP_LINEAR,
        unsigned int texture_unit = 0, glm::vec3 borderColor = glm::vec3(1.0, 0.0, 0.0));

    void use(unsigned int textureUnit) const;
    void clean();

    [[nodiscard]] bool isValid() const;

private:
    GLuint ID;
    bool isLoaded;

    static bool isMipMapRelatedFiltering(const FilterMode filterMode) {
        return filterMode == FilterMode::NEAREST_MIPMAP_NEAREST || filterMode == FilterMode::NEAREST_MIPMAP_LINEAR
    || filterMode == FilterMode::LINEAR_MIPMAP_NEAREST || filterMode == FilterMode::LINEAR_MIPMAP_LINEAR;
    }


};