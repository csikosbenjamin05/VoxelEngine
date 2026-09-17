//
// Created by beni on 2026. 09. 05..
//
#pragma once

// GLAD (included in shader.h) requires that its header be included before standard
// OpenGL headers or GLFW, otherwise it throws this explicit error.
// Tell GLFW not to bring in standard OpenGL headers so GLAD can handle function loading.
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "model/noise/PerlinNoise.h"
#include "model/noise/generators/MyTerrainGenerator.h"
#include "model/noise/generators/MyWorldGenerator.h"
#include "model/noise/generators/RandomFloatGenerator.h"
#include "rendering/wrapper/shader.h"
#include "rendering/wrapper/texture2D.h"
#include "rendering/wrapper/sampler2D.h"
#include "rendering/UpdateInfo.h"
#include "rendering/Camera.h"

#include "rendering/voxels/TextureAtlas.h"
#include "model/voxels/ChunkManager.h"
#include "rendering/geometry/BoundingBoxRenderer.h"
#include "rendering/voxels/VisualChunkManager.h"

struct ImGuiIO;

class MyApp {
public:

    MyApp();
    ~MyApp();

    bool Init();
    void Clean();

    void Update(const SUpdateInfo&);
    void Render() const;
    void RenderGUI(const ImGuiIO &im_gui_io);

    void SetResolution(int width, int height);
    void ResolutionChangedEvent();
    void MouseClickEvent(GLFWwindow* window, double xpos, double yPosIn);
    void MouseScrollEvent(GLFWwindow* window, double xOffset, double yOffset);
    void KeyEvent(GLFWwindow *window, const SUpdateInfo &updateInfo);

    bool enableBoundingBoxRenderer = false;

protected:
    void RenderGeometry() const;

    // Shader initialization and termination
    void InitShaders();
    void CleanShaders();

    // Geometry initialization and termination
    void InitGeometry();
    void CleanGeometry();

    // Texture initialization and termination
    void InitTextures();
    void CleanTextures();


private:

    int SCR_WIDTH{};
    int SCR_HEIGHT{};

    TextureAtlas texture_atlas;
    //ChunkData chunk_data;
    //VisualChunk visual_chunk;

    Camera camera;

    unsigned int VBO = 0, VAO = 0, EBO = 0;
    Shader myShader;
    //Shader voxelShader;
    Texture2D myTexture;
    Sampler2D mySampler;

    BoundingBoxRenderer bounding_box_renderer;

    RandomFloatGenerator randomFloatGenerator;
    PerlinNoise perlinNoise;
    MyTerrainGenerator my_terrain_generator;
    MyWorldGenerator my_world_generator;

    ChunkManager chunk_manager;
    VisualChunkManager visual_chunk_manager;

    bool frustumFollowsCamera = true;
};