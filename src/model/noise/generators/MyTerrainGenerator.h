#pragma once


#include <glm/vec2.hpp>
#include "model/noise/WorldGeneration.h"
#include "model/noise/layers/PerlinLayer.h"


class BlockType;
class RandomFloatGenerator;
class PerlinNoise;

class MyTerrainGenerator : public ITerrainGenerator 
{
public:
	MyTerrainGenerator(PerlinNoise* perlinNoise, RandomFloatGenerator* rfg);

	void SetSeed(RandomFloatGenerator* rfg) override; 

	[[nodiscard]] const VoxelType* GetVoxelTypeAt(glm::ivec3& voxelCoordinate, int surfaceHeight) const override;
	[[nodiscard]] int GetSurfaceHeight(int x, int z) const override;
private:
	PerlinLayer2D baseHeightLayer;
	PerlinLayer2D detailHeightLayer;
	PerlinLayer2D detailAmplitudeLayer;
};