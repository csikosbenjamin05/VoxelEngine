#pragma once

#include <model/noise/WorldGeneration.h>
#include "model/noise/generators/RandomFloatGenerator.h"


// Abstract
template <std::size_t Dim>
class ScalableLayer : public ILayerND<Dim>
{
public:
	using Coordinate = glm::vec<Dim, float>;

	explicit ScalableLayer(Coordinate& offset, float frequency = 1.0);

	explicit ScalableLayer(RandomFloatGenerator* rfg, float frequency = 1.0);

	float GetValueAt(const Coordinate& point) const override {
		return Calculate((point + offset) * frequency);
	}

	void SetSeed(RandomFloatGenerator* rfg) override;


protected:
	virtual float Calculate(const Coordinate& point) const = 0;

private:
	float frequency;
	Coordinate offset;
};

template<std::size_t Dim>
ScalableLayer<Dim>::ScalableLayer(Coordinate& offset, const float frequency)
: frequency(frequency), offset(offset)
{
}

template<std::size_t Dim>
ScalableLayer<Dim>::ScalableLayer(RandomFloatGenerator *rfg, const float frequency)
	: frequency(frequency) {
	ScalableLayer::SetSeed(rfg);
}

template<std::size_t Dim>
void ScalableLayer<Dim>::SetSeed(RandomFloatGenerator *rfg) {
	for (int d = 0; d < Dim; ++d)
		offset[d] = rfg->GetNextFloat() * ILayer::offsetRange;
}

using ScalableLayer1D = ScalableLayer<1>;
using ScalableLayer2D = ScalableLayer<2>;
using ScalableLayer3D = ScalableLayer<3>;