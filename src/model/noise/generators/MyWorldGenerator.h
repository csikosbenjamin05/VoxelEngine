#pragma once

#include "model/noise/WorldGeneration.h"

class Chunk;
class MyTerrainGenerator;

class MyWorldGenerator : public IWorldGenerator
{
public:
	explicit MyWorldGenerator(MyTerrainGenerator* terrainGen);
	void GenerateChunk(ChunkData* chunk) const override;

	void SetSeed(RandomFloatGenerator* rfg) override;

private:
	MyTerrainGenerator* terrainGenerator;
};