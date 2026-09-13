#include "PerlinNoise.h"

#include <model/noise/generators/RandomFloatGenerator.h>

const std::array<float, 720>& PerlinNoise::GetCosTable() {
    // Meyers Singleton
    static const std::array<float, 720> table = []() {
        std::array<float, 720> t{};

        for (int i = 0; i < 720; ++i)
            t[i] = std::cos(i * 0.017453292f * 0.5f);

        return t;
        }();

    return table;
}

PerlinNoise::PerlinNoise(RandomFloatGenerator* rfg)
    : octaves(4), fallOff(0.5f)
{
    PerlinNoise::SetSeed(rfg);
}

PerlinNoise::PerlinNoise(const int lod, const float fallOff, RandomFloatGenerator* rfg) : PerlinNoise(rfg)
{
    NoiseDetail(lod, fallOff);
}

void PerlinNoise::SetSeed(RandomFloatGenerator* rfg)
{
    for (int i = 0; i < 4096; ++i)
        perlin[i] = rfg->GetNextFloat();
}

float PerlinNoise::Noise(float x, float y, float z) const
{
    if (x < 0.0F) x = -x;
    if (y < 0.0F) y = -y;
    if (z < 0.0F) z = -z;

    int xi = static_cast<int>(x);
    int yi = static_cast<int>(y);
    int zi = static_cast<int>(z);
    float xf = x - xi;
    float yf = y - yi;
    float zf = z - zi;
    float r = 0.0F;
    float ampl = 0.5F;

    for (int i = 0; i < octaves; ++i)
    {
        int of = xi + (yi << 4) + (zi << 8);
        const float rxf = Noise_fsc(xf);
        const float ryf = Noise_fsc(yf);

        float n1 = perlin[of & 4095];
        n1 += rxf * (perlin[of + 1 & 4095] - n1);

        float n2 = perlin[of + 16 & 4095];
        n2 += rxf * (perlin[of + 16 + 1 & 4095] - n2);
        n1 += ryf * (n2 - n1);
        of += 256;
        n2 = perlin[of & 4095];
        n2 += rxf * (perlin[of + 1 & 4095] - n2);

        float n3 = perlin[of + 16 & 4095];
        n3 += rxf * (perlin[of + 16 + 1 & 4095] - n3);
        n2 += ryf * (n3 - n2);
        n1 += Noise_fsc(zf) * (n2 - n1);
        r += n1 * ampl;
        ampl *= fallOff;
        xi <<= 1;
        xf *= 2.0F;
        yi <<= 1;
        yf *= 2.0F;
        zi <<= 1;
        zf *= 2.0F;

        if (xf >= 1.0F)
        {
            ++xi;
            --xf;
        }

        if (yf >= 1.0F)
        {
            ++yi;
            --yf;
        }

        if (zf >= 1.0F)
        {
            ++zi;
            --zf;
        }
    }

    return r;
}

void PerlinNoise::NoiseDetail(const int lod)
{
    if (lod > 0)
        octaves = lod;
}

void PerlinNoise::NoiseDetail(const int lod, const float falloff)
{
    if (lod > 0)
        octaves = lod;

    if (falloff > 0.0F)
        fallOff = falloff;
}


float PerlinNoise::Noise_fsc(const float i)
{
    return 0.5F * (1.0F - GetCosTable()[static_cast<int>(i * PI) % TWO_PI]);
}