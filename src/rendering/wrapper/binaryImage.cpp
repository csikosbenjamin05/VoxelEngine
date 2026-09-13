#include "binaryImage.h"

#define STB_IMAGE_IMPLEMENTATION
#include <cstring>

#include "images/stb_image.h"

#include <iostream>


BinaryImage::BinaryImage(const std::filesystem::path& path, const bool flipVertically) {

    stbi_set_flip_vertically_on_load(flipVertically);

    data = stbi_load(path.string().c_str(), &width, &height, &colorChannels, 0);
    if (!data) {
        std::cerr << "Failed to load texture: " << path << "\n"
                  << "Reason: " << stbi_failure_reason() << std::endl;
        width = height = colorChannels = 0;
    }
}

BinaryImage::~BinaryImage() {
    if (data) {
        stbi_image_free(data);
        data = nullptr;
    }
}

BinaryImage::BinaryImage(BinaryImage&& other) noexcept
    : width(other.width),
      height(other.height),
      colorChannels(other.colorChannels),
      data(other.data)
{
    other.data = nullptr;
    other.width = other.height = other.colorChannels = 0;
}

BinaryImage& BinaryImage::operator=(BinaryImage&& other) noexcept {
    if (this != &other) {
        if (data) {
            stbi_image_free(data);
        }

        width = other.width;
        height = other.height;
        colorChannels = other.colorChannels;
        data = other.data;

        other.data = nullptr;
        other.width = other.height = other.colorChannels = 0;
    }
    return *this;
}

void BinaryImage::SliceIntoLayers(const int tileWidth, const int tileHeight) {
    const int cols = width / tileWidth;
    const int rows = height / tileHeight;
    layerCount = cols * rows;

    // Allocate new memory for the sliced 3D layout (layer -> y -> x)
    auto* newData = static_cast<unsigned char*>(malloc(width * height * colorChannels));

    int destOffset = 0;
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            for (int y = 0; y < tileHeight; ++y) {
                const int srcY = r * tileHeight + y;
                const int srcX = c * tileWidth;
                const int srcOffset = (srcY * width + srcX) * colorChannels;
                const int rowBytes = tileWidth * colorChannels;

                std::memcpy(newData + destOffset, data + srcOffset, rowBytes);
                destOffset += rowBytes;
            }
        }
    }

    stbi_image_free(data); // Free the original STB image
    data = newData;        // Assign the tightly packed layered data
    width = tileWidth;     // Update dimensions to represent a single layer
    height = tileHeight;
}

int BinaryImage::NumberOfMIPLevels() const {
    int targetLevel = 1;
    unsigned int index = std::max( width, height );

    while (index >>= 1) ++targetLevel;

    return targetLevel;
}
