//
// Created by beni on 2026. 09. 05..
//

#pragma once

#include "model/voxels/ChunkData.h"
#include "rendering/wrapper/meshData.h"

class VisualChunk : public DrawableMesh {
    public:
    explicit VisualChunk(ChunkData* newChunk);
    ~VisualChunk() override;

    void Bake();
    void Init() override;

    void SetChunkReference(ChunkData* newChunk);

    void GenerateInstanced(const std::vector<FaceData> &faces);

    void DrawObject() const override;

    [[nodiscard]] BoundingBox& GetBoundingBoxReference() {
        return chunk->getBoundingBox();
    }


private:
    ChunkData* chunk;


    /*
    static void CreateFace(MeshObject<VertexPos3Tex2> &meshCPU,
        glm::ivec3 position, DIRECTION direction, TextureCoordinate tc
    );
    */
};

