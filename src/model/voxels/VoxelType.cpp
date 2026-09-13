//
// Created by beni on 2026. 09. 05..
//
#include "VoxelType.h"
#include <utility>

VoxelType::VoxelType(std::string name, const char symbol, const uint16_t id, const bool isSolid)
    : name(std::move(name)), symbol(symbol), id(id), isSolid(isSolid)
{
    GetRegistry()[id] = this;
}

const VoxelType VoxelType::Air{ "Air", '.', 0, false};
const VoxelType VoxelType::Grass{ "Grass", 'G', 1, true};
const VoxelType VoxelType::Dirt{ "Dirt", 'd', 2, true};
const VoxelType VoxelType::Stone{ "Stone", 's', 3, true};


const VoxelType* VoxelType::FromID(const uint16_t id) {
    auto& registry = GetRegistry();

    if (const auto it = registry.find(id); it != registry.end())
        return it->second;
    return nullptr;
}

std::unordered_map<uint16_t, const VoxelType*>& VoxelType::GetRegistry()
{
    static std::unordered_map<uint16_t, const VoxelType*> registry;
    return registry;
}

std::string VoxelType::toString() const { return name; }

std::ostream& operator<<(std::ostream& os, const VoxelType& a)
{
    os << a.name;
    return os;
}