#include "MyWorldGenerator.h"

#include <model/voxels/ChunkData.h>
#include <model/voxels/VoxelType.h>
#include <model/noise/generators/MyTerrainGenerator.h>


MyWorldGenerator::MyWorldGenerator(MyTerrainGenerator* terrainGen)
    : terrainGenerator(terrainGen)
{
}

void MyWorldGenerator::GenerateChunk(ChunkData* chunk) const
{
    const auto position = chunk->getChunkPosition() * ChunkData::SIZE;

    VoxelData voxelData;
    for (int z = 0; z < ChunkData::SIZE; z++) {
        for (int x = 0; x < ChunkData::SIZE; x++) {
            const int surface = terrainGenerator->GetSurfaceHeight(x, z);
            for (int y = 0; y < ChunkData::SIZE; y++) {

                auto voxel_local_coordinate = glm::ivec3(x,y,z);

                auto voxel_world_coordinate = position + voxel_local_coordinate;
                const auto voxelType = terrainGenerator->GetVoxelTypeAt(voxel_world_coordinate, surface);

                voxelData.setID(voxelType->id);
                chunk->set(voxel_local_coordinate, voxelData);

            }
        }
    }
}

void MyWorldGenerator::SetSeed(RandomFloatGenerator *rfg) {
    terrainGenerator->SetSeed(rfg);
}
