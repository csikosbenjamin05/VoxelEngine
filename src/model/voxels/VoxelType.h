//
// Created by beni on 2026. 09. 05..
//
#pragma once

#include <string>
#include <unordered_map>
#include <iosfwd>

class VoxelType {

public:
    static const VoxelType Air;
    static const VoxelType Grass;
    static const VoxelType Dirt;
    static const VoxelType Stone;

    const std::string name;
    const char symbol;
    const uint16_t id;
    const bool isSolid;

    VoxelType(std::string name, char symbol, uint16_t id, bool isSolid = true);

    [[nodiscard]] std::string toString() const;
    friend std::ostream& operator<<(std::ostream& os, const VoxelType& a);

    static const VoxelType* FromID(uint16_t id);
    static std::unordered_map<uint16_t, const VoxelType*>& GetRegistry();
};