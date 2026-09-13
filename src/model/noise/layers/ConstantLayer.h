#pragma once

#include <model/noise/WorldGeneration.h>

template <std::size_t Dim>
class ConstantLayer : public ILayerND<Dim>
{
public:
	using Point = ILayerND<Dim>::Point;

	explicit ConstantLayer(const float height) : height(height)
	{
	}

	float GetValueAt(const Point& point) const override
	{
		return height;
	}

private:
	float height;
};

using ConstantLayer1D = ConstantLayer<1>;
using ConstantLayer2D = ConstantLayer<1>;
using ConstantLayer3D = ConstantLayer<1>;
