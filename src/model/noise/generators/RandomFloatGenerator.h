#pragma once

#include <random>

class RandomFloatGenerator {
public:
	explicit RandomFloatGenerator(unsigned int seed, float lower = 0.0f, float upper = 1.0f);

	[[nodiscard]] unsigned int GetSeed() const
	{
		return seed;
	}

	void SetSeed(unsigned int seed);

	[[nodiscard]] float GetNextFloat();
	

private:
	unsigned int seed;
	std::mt19937 generator;
	std::uniform_real_distribution<float> distribution;
};