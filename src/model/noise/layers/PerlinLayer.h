#pragma once

#include "ScalableLayer.h"
#include "model/noise/PerlinNoise.h"

class RandomFloatGenerator;
class PerlinNoise;

// Generic layered terrain for any dimension
template <std::size_t Dim>
class PerlinLayer : public ScalableLayer<Dim>
{
public:
	using Coordinate = glm::vec<Dim, float>;

	PerlinLayer(PerlinNoise* perlin, Coordinate& offset, float frequency = 1.0)
	: ScalableLayer<Dim>(offset, frequency), perlin(perlin)
	{
	}

	PerlinLayer(PerlinNoise* perlin, RandomFloatGenerator* rfg, float frequency = 1.0)
	: ScalableLayer<Dim>(rfg, frequency), perlin(perlin)
	{
	}

	void SetSeed(RandomFloatGenerator* rfg) override {
		ScalableLayer<Dim>::SetSeed(rfg);
	}

protected:
	float Calculate(const Coordinate& point) const override {
		return perlin->NoiseND<Dim>(point);
	}

private:
	PerlinNoise* perlin;
};

using PerlinLayer1D = PerlinLayer<1>;
using PerlinLayer2D = PerlinLayer<2>;
using PerlinLayer3D = PerlinLayer<3>;
