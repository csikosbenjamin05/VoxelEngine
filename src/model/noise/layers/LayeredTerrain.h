#pragma once

#include <vector>

#include <model/noise/WorldGeneration.h>

// Generic layered terrain for any dimension
template <std::size_t Dim>
class LayeredTerrain : public ILayerND<Dim>
{
public:
	using Coordinate = glm::vec<Dim, float>::Point;

	explicit LayeredTerrain(size_t reservedLayers = 0)
	{
		layers.reserve(reservedLayers);
	}

	void AddLayer(ILayerND<Dim>* layer)
	{
		layers.push_back(layer);
	}

	float GetValueAt(const Coordinate& point) const override
	{
		float total = 0.0f;
		for (const auto* layer : layers)
		{
			if (layer)
				total += layer->GetValueAt(point);
		}
		return total;
	}

private:
	std::vector<ILayerND<Dim>*> layers;
};

using LayeredTerrain1D = LayeredTerrain<1>;
using LayeredTerrain2D = LayeredTerrain<2>;
using LayeredTerrain3D = LayeredTerrain<3>;