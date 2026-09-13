#include "RandomFloatGenerator.h"


RandomFloatGenerator::RandomFloatGenerator(const unsigned int seed, const float lower, const float upper)
	: seed(seed), generator(seed), distribution(lower, upper)
{

}

[[nodiscard]] float RandomFloatGenerator::GetNextFloat()
{
	return distribution(generator);
}

void RandomFloatGenerator::SetSeed(unsigned int newSeed)
{
    seed = newSeed;
    generator.seed(seed);
    distribution.reset();
}