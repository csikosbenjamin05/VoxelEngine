//
// Created by beni on 2026. 09. 04..
//
#pragma once
#include <filesystem>

struct BinaryImage {

    int width = 0;
    int height = 0;
    int layerCount = 1;
    int colorChannels = 0;
    unsigned char* data = nullptr;

    explicit BinaryImage(const std::filesystem::path& path, bool flipVertically = true);
    ~BinaryImage();

    // Prevent double-free (disable copying)
    BinaryImage(const BinaryImage&) = delete;
    BinaryImage& operator=(const BinaryImage&) = delete;

    // Support moving (transfer ownership safely)
    BinaryImage(BinaryImage&& other) noexcept;
    BinaryImage& operator=(BinaryImage&& other) noexcept;

    void SliceIntoLayers(int tileWidth, int tileHeight);

    [[nodiscard]] bool isValid() const { return data != nullptr; }
    explicit operator bool() const { return isValid(); }

    [[nodiscard]] int NumberOfMIPLevels() const;

};