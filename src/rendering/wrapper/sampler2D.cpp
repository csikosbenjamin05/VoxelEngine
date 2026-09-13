//
// Created by beni on 2026. 09. 05..
//

#include "sampler2D.h"

#include <iostream>
#include <glm/gtc/type_ptr.hpp>

Sampler2D::Sampler2D() : ID(0), isLoaded(false) {}

Sampler2D::~Sampler2D() {
    clean();
}

void Sampler2D::init(WrapMode wrap_S, WrapMode wrap_T, FilterMode magnifyingFilter, FilterMode minifyingFilter,
                          const unsigned int texture_unit, glm::vec3 borderColor) {

    if (texture_unit > 15 || isMipMapRelatedFiltering(magnifyingFilter)) {
        std::cout << "INVALID INIT PARAMETERS FOR SAMPLER, Failed to create sampler" << std::endl;
        return;
    }

    // set the texture wrapping/filtering options (on the currently bound texture object)
    glCreateSamplers(1, &ID);
    glSamplerParameteri(ID, GL_TEXTURE_WRAP_S, static_cast<GLint>(wrap_S));
    glSamplerParameteri(ID, GL_TEXTURE_WRAP_T, static_cast<GLint>(wrap_T));

    glSamplerParameteri(ID, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(minifyingFilter));
    glSamplerParameteri(ID, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(magnifyingFilter));

    if (wrap_S == WrapMode::CLAMP_TO_BORDER || wrap_T == WrapMode::CLAMP_TO_BORDER) {
        glSamplerParameterfv(ID, GL_TEXTURE_BORDER_COLOR, glm::value_ptr(borderColor));
    }

    isLoaded = true;
}

void Sampler2D::use(const unsigned int textureUnit) const {
    glBindSampler(textureUnit, ID);
}

void Sampler2D::clean() {
    if (isLoaded)
        glDeleteSamplers(1, &ID);
    isLoaded = false;
}

bool Sampler2D::isValid() const {
    return isLoaded;
}
