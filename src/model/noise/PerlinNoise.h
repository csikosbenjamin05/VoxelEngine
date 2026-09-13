#pragma once

#include <array>
#include "WorldGeneration.h"

class RandomFloatGenerator;

// Large part of this Perlin noise generation was taken from Processing 4 then converted to C++
class PerlinNoise : ISeedable
{
public:
    explicit PerlinNoise(RandomFloatGenerator* rfg);
    PerlinNoise(int lod, float fallOff, RandomFloatGenerator* rfg);

    void SetSeed(RandomFloatGenerator* rfg) override;

    template <std::size_t Dim>
    float NoiseND(const glm::vec<Dim, float>& point) const {
        if constexpr (Dim == 0) {
            return 0.0f;
        } else if constexpr (Dim == 1) {
            return Noise(point.x);
        } else if constexpr (Dim == 2) {
            return Noise(point.x, point.y);
        } else {
            return Noise(point.x, point.y, point.z);
        }
    }

    [[nodiscard]] float Noise(float x, float y = 0.0F, float z = 0.0F) const;

    void NoiseDetail(int lod);

    void NoiseDetail(int lod, float falloff);

private:

    static constexpr int PI = 360;
    static constexpr int TWO_PI = 720;

    static const std::array<float, 720>& GetCosTable();

    static float Noise_fsc(float i);

    std::array<float, 4096> perlin;
    int octaves;
    float fallOff;

};