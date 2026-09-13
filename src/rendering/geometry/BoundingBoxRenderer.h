//
// Created by beni on 2026. 09. 12..
//
#pragma once

#include <unordered_set>
#include "rendering/wrapper/meshData.h"
#include "rendering/wrapper/shader.h"

/*
Renders bounding boxes added by reference



*/
class BoundingBoxRenderer : public DrawableMesh {
public:
    BoundingBoxRenderer();
    ~BoundingBoxRenderer() override;

    void Init() override;
    void DrawObject() const override;

    void AddBoundingBoxReference(const BoundingBox* boundingBox);
    void RemoveBoundingBoxReference(const BoundingBox* boundingBox);
    void SetViewProjMatrix(const glm::mat4& viewProj) {
        viewProjMatrix = viewProj;
    }

protected:

    void CleanOther() override;

    static constexpr auto RED = glm::vec3(1.0f, 0.0f, 0.0f);
    static constexpr auto GREEN = glm::vec3(0.0f, 1.0f, 0.0f);
    static constexpr auto BLUE = glm::vec3(0.0f, 0.0f, 1.0f);
    static constexpr auto YELLOW = glm::vec3(1.0f, 1.0f, 0.0f);
    static constexpr auto MAGENTA = glm::vec3(1.0f, 0.0f, 1.0f);
    static constexpr auto CYAN = glm::vec3(0.0f, 1.0f, 1.0f);


private:


    std::unordered_set<const BoundingBox*> boundingBoxes;
    glm::mat4 viewProjMatrix {1.0f};
    Shader boundingBoxShader;
};

