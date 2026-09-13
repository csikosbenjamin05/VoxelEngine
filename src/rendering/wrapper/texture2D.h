//
// Created by beni on 2026. 09. 05..
//
#pragma once

#include <filesystem>

struct Texture2D {

    Texture2D();
    ~Texture2D();

    Texture2D(const Texture2D&) = delete;
    Texture2D& operator=(const Texture2D&) = delete;
    Texture2D(Texture2D&&) noexcept = default;
    Texture2D& operator=(Texture2D&&) noexcept = default;

    void init(const std::filesystem::path& path, bool genMipMap);
    void use(unsigned int textureUnit) const;
    void clean();

    [[nodiscard]] int getWidth() const {return width;}
    [[nodiscard]] int getHeight() const {return height;}

    [[nodiscard]] bool isValid() const {return isLoaded;}

private:
    unsigned int ID;
    int width, height;
    bool isLoaded;


};

struct Texture2DArray{

    Texture2DArray();
    ~Texture2DArray();

    Texture2DArray(const Texture2DArray&) = delete;
    Texture2DArray& operator=(const Texture2DArray&) = delete;
    Texture2DArray(Texture2DArray&&) noexcept = default;
    Texture2DArray& operator=(Texture2DArray&&) noexcept = default;

    void init(const std::filesystem::path& path, int tileWidth, int tileHeight, bool genMipMap);
    void use(unsigned int textureUnit) const;
    void clean();

    [[nodiscard]] int getWidth() const {return width;}
    [[nodiscard]] int getHeight() const {return height;}

    [[nodiscard]] bool isValid() const {return isLoaded;}

private:
    unsigned int ID;
    int width, height;
    int layers;
    bool isLoaded;

};