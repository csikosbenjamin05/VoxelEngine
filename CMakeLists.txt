cmake_minimum_required(VERSION 4.3)
project(untitled)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Find Debian system packages installed via apt
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)

# 2. Define all source files to compile
set(SOURCES
        src/main.cpp
        src/MyApp.cpp

        src/external/glad.c
        src/external/imgui/imgui.cpp
        src/external/imgui/imgui_draw.cpp
        src/external/imgui/imgui_widgets.cpp
        src/external/imgui/imgui_tables.cpp
        src/external/imgui/backends/imgui_impl_glfw.cpp
        src/external/imgui/backends/imgui_impl_opengl3.cpp

        src/rendering/wrapper/shader.cpp
        src/rendering/wrapper/binaryImage.cpp
        src/rendering/wrapper/meshData.cpp
        src/rendering/wrapper/texture2D.cpp
        src/rendering/wrapper/sampler2D.cpp

        src/model/math/intersections.cpp
        src/rendering/geometry/rectangularPrism.h
        src/rendering/voxels/VisualChunk.cpp
        src/rendering/voxels/TextureAtlas.cpp
        src/rendering/Camera.cpp

        src/model/voxels/VoxelType.cpp
        src/model/voxels/ChunkData.cpp

        src/model/noise/PerlinNoise.cpp
        src/model/noise/generators/RandomFloatGenerator.cpp
        src/model/noise/generators/MyTerrainGenerator.cpp
        src/model/noise/generators/MyWorldGenerator.cpp



        src/model/math/BoundingBox.cpp
        src/rendering/VoxelShader.cpp
        src/rendering/VoxelShader.h
        src/model/math/Frustum.cpp
        src/rendering/geometry/BoundingBoxRenderer.cpp

)
# src/model/voxels/ChunkManager.cpp

# 3. Create the executable
add_executable(${PROJECT_NAME} ${SOURCES})

# 4. Configure include directories (target exists now)
target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        include
        include/imgui
        include/imgui/backends
)

# 5. Copy the shaders directory to the build output directory
add_custom_target(copy_shaders ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/shaders
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders
)
add_dependencies(${PROJECT_NAME} copy_shaders)

# 5. Copy the resources directory to the build output directory
add_custom_target(copy_resources ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/resources
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources
)
add_dependencies(${PROJECT_NAME} copy_resources)

# 6. Link the system libraries to the executable
target_link_libraries(${PROJECT_NAME} glfw OpenGL::GL glm::glm)