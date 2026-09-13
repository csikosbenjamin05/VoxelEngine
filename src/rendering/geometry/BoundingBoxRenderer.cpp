//
// Created by beni on 2026. 09. 12..
//

#include "BoundingBoxRenderer.h"

#include <glm/gtc/type_ptr.hpp>


BoundingBoxRenderer::BoundingBoxRenderer() = default;

BoundingBoxRenderer::~BoundingBoxRenderer() = default;

void BoundingBoxRenderer::Init() {

    boundingBoxShader.initShader("shaders/boundingBox.vert", "shaders/boundingBox.frag");

    MeshObject<VertexPos3> meshCPU;
    meshCPU.vertexArray = {
            {glm::vec3(0, 0, 0)}, {glm::vec3(1, 0, 0)}, {glm::vec3(1, 1, 0)}, {glm::vec3(0, 1, 0)}, // Bottom
            {glm::vec3(0, 0, 1)}, {glm::vec3(1, 0, 1)}, {glm::vec3(1, 1, 1)}, {glm::vec3(0, 1, 1)}  // Top
    };

    // 12 lines (24 indices) to connect the corners
    meshCPU.indexArray = {
        0,1, 1,2, 2,3, 3,0, // Bottom square
        4,5, 5,6, 6,7, 7,4, // Top square
        0,4, 1,5, 2,6, 3,7  // Vertical pillars
    };
    GenerateFromMesh(meshCPU, GetVertexAttribListPos3());
}

void BoundingBoxRenderer::DrawObject() const {
    boundingBoxShader.use();
    glBindVertexArray(gpuObject.vaoID);

    glUniformMatrix4fv(
        boundingBoxShader.getUniformLocation("viewProj"),
        1,
        GL_FALSE,
        glm::value_ptr(viewProjMatrix)
    );

    for (const auto* box : boundingBoxes) {
        auto pos = glm::vec3(box->getMinCorner());
        auto size = glm::vec3(box->getSize());

        auto transform = glm::translate(glm::mat4(1.0f), pos) * glm::scale(glm::mat4(1.0f), size);
        glUniformMatrix4fv(ul(boundingBoxShader.getProgramID(),"world"), 1, GL_FALSE, glm::value_ptr(transform));
        glUniform3fv(ul(boundingBoxShader.getProgramID(),"color"), 1, glm::value_ptr(RED));

        glDrawElements(GL_LINES, gpuObject.count, GL_UNSIGNED_INT, nullptr);
    }

    glUseProgram(0);
    glBindVertexArray(0);
}

void BoundingBoxRenderer::CleanOther() {
    boundingBoxShader.deleteShader();
}

void BoundingBoxRenderer::AddBoundingBoxReference(const BoundingBox *boundingBox) {
    boundingBoxes.insert(boundingBox);
}

void BoundingBoxRenderer::RemoveBoundingBoxReference(const BoundingBox *boundingBox) {
    boundingBoxes.erase(boundingBox);
}

/*
void BoundingBoxRenderer::CreateFace(MeshObject<VertexPos3> &meshCPU, const glm::ivec3 position, const DIRECTION direction) {

    const auto x = static_cast<float>(position.x);
    const auto y = static_cast<float>(position.y);
    const auto z = static_cast<float>(position.z);

    const float x2 = x + 1.0f;
    const float y2 = y + 1.0f;
    const float z2 = z + 1.0f;

    const int startIndex = static_cast<int>(meshCPU.vertexArray.size());

    // Vertices order: Bottom-Left, Bottom-Right, Top-Right, Top-Left
    switch (direction) {
        case posY: // Top Face (+Y)
            meshCPU.vertexArray.push_back({glm::vec3(x,  y2, z2)});
            meshCPU.vertexArray.push_back({glm::vec3(x2, y2, z2)});
            meshCPU.vertexArray.push_back({glm::vec3(x2, y2, z )});
            meshCPU.vertexArray.push_back({glm::vec3(x,  y2, z )});
            break;
        case negY: // Bottom Face (-Y)
            meshCPU.vertexArray.push_back({glm::vec3(x,  y,  z )});
            meshCPU.vertexArray.push_back({glm::vec3(x2, y,  z )});
            meshCPU.vertexArray.push_back({glm::vec3(x2, y,  z2)});
            meshCPU.vertexArray.push_back({glm::vec3(x,  y,  z2)});
            break;
        case posZ: // Front Face (+Z)
            meshCPU.vertexArray.push_back({glm::vec3(x,  y,  z2)});
            meshCPU.vertexArray.push_back({glm::vec3(x2, y,  z2)});
            meshCPU.vertexArray.push_back({glm::vec3(x2, y2, z2)});
            meshCPU.vertexArray.push_back({glm::vec3(x,  y2, z2)});
            break;
        case negZ: // Back Face (-Z)
            meshCPU.vertexArray.push_back({glm::vec3(x2, y,  z )});
            meshCPU.vertexArray.push_back({glm::vec3(x,  y,  z )});
            meshCPU.vertexArray.push_back({glm::vec3(x,  y2, z )});
            meshCPU.vertexArray.push_back({glm::vec3(x2, y2, z )});
            break;
        case posX: // Right Face (+X)
            meshCPU.vertexArray.push_back({glm::vec3(x2, y,  z2)});
            meshCPU.vertexArray.push_back({glm::vec3(x2, y,  z )});
            meshCPU.vertexArray.push_back({glm::vec3(x2, y2, z )});
            meshCPU.vertexArray.push_back({glm::vec3(x2, y2, z2)});
            break;
        case negX: // Left Face (-X)
            meshCPU.vertexArray.push_back({glm::vec3(x,  y,  z )});
            meshCPU.vertexArray.push_back({glm::vec3(x,  y,  z2)});
            meshCPU.vertexArray.push_back({glm::vec3(x,  y2, z2)});
            meshCPU.vertexArray.push_back({glm::vec3(x,  y2, z )});
            break;
    }

    // CCW Triangle 1
    meshCPU.indexArray.push_back(startIndex + 0);
    meshCPU.indexArray.push_back(startIndex + 1);
    meshCPU.indexArray.push_back(startIndex + 2);

    // CCW Triangle 2
    meshCPU.indexArray.push_back(startIndex + 0);
    meshCPU.indexArray.push_back(startIndex + 2);
    meshCPU.indexArray.push_back(startIndex + 3);
}
*/