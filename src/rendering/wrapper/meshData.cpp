//
// Created by beni on 2026. 09. 04..
//

#include "meshData.h"

#include <glm/gtc/type_ptr.hpp>
#include <cassert>

#include "shader.h"

//----------------------------------------------------------
// DrawableObject
//----------------------------------------------------------


DrawableMesh::DrawableMesh()
    : world(glm::mat4(1.0)), invWorld(glm::mat4(1.0))
{
}

void DrawableMesh::Clean()
{
    CleanOGLObject(gpuObject);
    CleanOther();
}

void DrawableMesh::CleanOGLObject( OGLObject& ObjectGPU )
{
    glDeleteBuffers(1,      &ObjectGPU.vboID);
    ObjectGPU.vboID = 0;
    glDeleteBuffers(1,      &ObjectGPU.iboID);
    ObjectGPU.iboID = 0;
    glDeleteVertexArrays(1, &ObjectGPU.vaoID);
    ObjectGPU.vaoID = 0;
}

void DrawableMesh::SetWorldTransform(const glm::mat4& trf)
{
    world = trf;
    invWorld = glm::transpose(glm::inverse(world));
}

const glm::mat4& DrawableMesh::GetWorldTransform() const
{
    return world;
}

void DrawableMesh::DrawObject() const
{
    glUniformMatrix4fv(ul("world"), 1, GL_FALSE, glm::value_ptr(world));
    glUniformMatrix4fv(ul("worldInvTranspose"), 1, GL_FALSE, glm::value_ptr(invWorld));
    glBindVertexArray(gpuObject.vaoID);

    glDrawElements(GL_TRIANGLES, gpuObject.count, GL_UNSIGNED_INT, nullptr);

    glBindVertexArray(0);
}


const std::initializer_list<VertexAttributeDescriptor> & DrawableMesh::GetVertexAttribListPos3Tex2() {

    // Meyers Singleton
    static constexpr std::initializer_list<VertexAttributeDescriptor> vertexAttribList =
    {
        { 0, offsetof(VertexPos3Tex2, position), 3, GL_FLOAT },
           { 1, offsetof(VertexPos3Tex2, texCoord),	2, GL_FLOAT },
    };

    return vertexAttribList;

}

const std::initializer_list<VertexAttributeDescriptor>& DrawableMesh::GetVertexAttribListPos3Norm3Tex2()
{
    // Meyers Singleton
    static constexpr std::initializer_list<VertexAttributeDescriptor> vertexAttribList =
    {
        { 0, offsetof(VertexPos3Norm3tTex2, position), 3, GL_FLOAT },
           { 1, offsetof(VertexPos3Norm3tTex2, normal),	 3, GL_FLOAT },
           { 2, offsetof(VertexPos3Norm3tTex2, texCoord), 2, GL_FLOAT },
    };

    return vertexAttribList;
}

const std::initializer_list<VertexAttributeDescriptor>& DrawableMesh::GetVertexAttribListPos2Tex2Light1()
{
    // Meyers Singleton
    static constexpr std::initializer_list<VertexAttributeDescriptor> vertexAttribList =
    {
        { 0, offsetof(VertexPos2Tex2Light1, position),	2, GL_FLOAT },
           { 1, offsetof(VertexPos2Tex2Light1, texCoord),	2, GL_FLOAT },
           { 2, offsetof(VertexPos2Tex2Light1, light),		1, GL_FLOAT },
    };

    return vertexAttribList;
}

const std::initializer_list<VertexAttributeDescriptor>& DrawableMesh::GetVertexAttribListPos2Tex2()
{
    // Meyers Singleton
    static constexpr std::initializer_list<VertexAttributeDescriptor> vertexAttribList =
    {
        { 0, offsetof(VertexPos2Tex2, position),	2, GL_FLOAT },
           { 1, offsetof(VertexPos2Tex2, texCoord),	2, GL_FLOAT }
    };

    return vertexAttribList;
}

const std::initializer_list<VertexAttributeDescriptor>& DrawableMesh::GetVertexAttribListPos3()
{
    // Meyers Singleton
    static constexpr std::initializer_list<VertexAttributeDescriptor> vertexAttribList =
    {
        { 0, offsetof(VertexPos3, position),	3, GL_FLOAT },
    };

    return vertexAttribList;
}