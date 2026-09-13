#include "MyTerrainGenerator.h"

#include <model/noise/generators/RandomFloatGenerator.h>
#include <model/noise/PerlinNoise.h>
#include <model/voxels/ChunkData.h>
#include <model/voxels/VoxelType.h>


MyTerrainGenerator::MyTerrainGenerator(PerlinNoise* perlinNoise, RandomFloatGenerator* rfg)
	: heightLayer(perlinNoise, rfg, 1.0f / 30.0f)
{
}

const VoxelType* MyTerrainGenerator::GetVoxelTypeAt(glm::ivec3& voxelCoordinate, const int surfaceHeight) const
{
	const int stone = surfaceHeight / 3 * 2; // 40%
	const int y = voxelCoordinate.y;

	if (y > surfaceHeight)
		return &VoxelType::Air;

	if (y == surfaceHeight)
		return &VoxelType::Grass;
	
	if (y > stone)
		return &VoxelType::Dirt;

	return &VoxelType::Stone;
}

void MyTerrainGenerator::SetSeed(RandomFloatGenerator* rfg)
{
	heightLayer.SetSeed(rfg);
}

int MyTerrainGenerator::GetSurfaceHeight(const int x, const int z) const
{
	return static_cast<int>(heightLayer.GetValueAt(glm::ivec2(x, z)) * ChunkData::SIZE);
}
