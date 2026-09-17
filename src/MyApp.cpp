//
// Created by beni on 2026. 09. 05..
//

#include "MyApp.h"

#include <iostream>
#include <glm/fwd.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui.h"

MyApp::MyApp() :
randomFloatGenerator(1), perlinNoise(&randomFloatGenerator)
, my_terrain_generator(&perlinNoise, &randomFloatGenerator)
, my_world_generator(&my_terrain_generator),
chunk_manager(&my_world_generator, glm::ivec3(0,0,0), 5, 2),
visual_chunk_manager(&chunk_manager, &texture_atlas)
{
    //chunk_data.generateNoise(glm::ivec3(0,0,0));
}
MyApp::~MyApp() = default;


//--------------------------------------------------------
//                  INITIALISATION
//--------------------------------------------------------

bool MyApp::Init() {
    InitTextures();
    InitGeometry();
    InitShaders();

    bounding_box_renderer.Init();
    //bounding_box_renderer.AddBoundingBoxReference(visual_chunk.GetBoundingBoxReference());

    visual_chunk_manager.Init();

    return true;
}

void MyApp::InitShaders() {
    myShader.initShader("shaders/vert.vert", "shaders/frag.frag");
    //voxelShader.initShader("shaders/voxel.vert", "shaders/voxel.frag");
}

void MyApp::InitGeometry() {


/*
    // 4. Define Triangle Vertices (Positions + RGB Colors)
    constexpr float vertices[] = {
        // positions          // normals           // texture coords
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
       -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
       -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left
   };

    constexpr unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };

    // 5. Setup VAO and VBO and EBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Setting up the element buffer object
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Position attribute (location = 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), static_cast<void *>(nullptr));
    glEnableVertexAttribArray(0);
    // Color attribute (location = 1)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // Texture coords (location = 2
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void *>(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Unbind VAO (safe practice)
    glBindVertexArray(0);
    */

    constexpr float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), static_cast<void *>(nullptr));
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

}

void MyApp::InitTextures() {
    myTexture.init("resources/wall.png", true);
    mySampler.init();
    texture_atlas.Init("resources/textures.png");
}

//--------------------------------------------------------
//                  UPDATING & RENDERING
//--------------------------------------------------------

void MyApp::Update(const SUpdateInfo & updateInfo) {

    if (frustumFollowsCamera) {

        camera.UpdateFrustum(static_cast<float>(SCR_WIDTH), static_cast<float>(SCR_HEIGHT));

        visual_chunk_manager.UpdateFrustum(camera.frustum);
    }

    static GLint transformLoc =  myShader.getUniformLocation("transform");

    constexpr auto model = glm::mat4(1.0f);
    //model = glm::rotate(model, updateInfo.ElapsedTimeInSec, glm::vec3(1.0f, 0.0f, 0.0f));

    const glm::mat4 viewProj = camera.GetViewProjMatrix(static_cast<float>(SCR_WIDTH), static_cast<float>(SCR_HEIGHT));

    // 3. Send the matrix to the shader
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(viewProj * model));
    bounding_box_renderer.SetViewProjMatrix(camera.GetViewProjMatrix(static_cast<float>(SCR_WIDTH), static_cast<float>(SCR_HEIGHT)));
}

void MyApp::Render() const {

    glClearColor(0.1f, 0.15f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    RenderGeometry();

    if (enableBoundingBoxRenderer) {
        glDisable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);
        bounding_box_renderer.DrawObject();
    }

}

void MyApp::RenderGUI(const ImGuiIO &im_gui_io) {
    ImGui::Begin("Settings");
    ImGui::Text("Hello, Debian 12!");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / im_gui_io.Framerate, im_gui_io.Framerate);
    ImGui::Checkbox("Frustum follows the camera", &frustumFollowsCamera);
    ImGui::End();
    // ImGui::ShowDemoWindow(); // Uncomment to see all ImGui features
}



void MyApp::RenderGeometry() const {

/*
    voxelShader.use();
    glUniformMatrix4fv(
        voxelShader.getUniformLocation("viewProj"),
        1,
        GL_FALSE,
        glm::value_ptr(camera.GetViewProjMatrix(static_cast<float>(SCR_WIDTH), static_cast<float>(SCR_HEIGHT)))
    );
    texture_atlas.UseVoxelAtlas(0);
    visual_chunk.DrawObject();
*/

    visual_chunk_manager.Render(camera.GetViewProjMatrix(static_cast<float>(SCR_WIDTH), static_cast<float>(SCR_HEIGHT)));


    myShader.use();
    mySampler.use(0);
    myTexture.use(0);
    glBindVertexArray(VAO);
    //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

}

//--------------------------------------------------------
//                  CLEANING
//--------------------------------------------------------
void MyApp::Clean() {
    CleanShaders();
    CleanTextures();
    CleanGeometry();

    bounding_box_renderer.Clean();

    visual_chunk_manager.Clean();
}

void MyApp::CleanShaders() {
    myShader.deleteShader();
}

void MyApp::CleanGeometry() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void MyApp::CleanTextures() {
    myTexture.clean();
    texture_atlas.Clean();
}

//--------------------------------------------------------
//                  EVENTS
//--------------------------------------------------------

void MyApp::SetResolution(const int width, const int height) {
    if (SCR_WIDTH != width || SCR_HEIGHT != height) {
        SCR_HEIGHT = height;
        SCR_WIDTH = width;
        ResolutionChangedEvent();
    }
}

void MyApp::ResolutionChangedEvent() {
    std::cout << "New resolution" << SCR_WIDTH << "*" << SCR_HEIGHT << std::endl;
}




void MyApp::MouseClickEvent(GLFWwindow *window, const double xPosIn, const double yPosIn) {

    static bool firstMouse = true;
    static float lastX = static_cast<float>(SCR_HEIGHT) / 2.0f;
    static float lastY = static_cast<float>(SCR_HEIGHT) / 2.0f;

    const auto xPos = static_cast<float>(xPosIn);
    const auto yPos = static_cast<float>(yPosIn);

    if (firstMouse)
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    const float xOffset = xPos - lastX;
    const float yOffset = lastY - yPos; // reversed since y-coordinates go from bottom to top

    lastX = xPos;
    lastY = yPos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}

void MyApp::MouseScrollEvent(GLFWwindow *window, const double xOffset, const double yOffset) {
    camera.ProcessMouseScroll(static_cast<float>(yOffset));
}

void MyApp::KeyEvent(GLFWwindow *window, const SUpdateInfo &updateInfo) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, updateInfo.DeltaTimeInSec);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, updateInfo.DeltaTimeInSec);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, updateInfo.DeltaTimeInSec);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, updateInfo.DeltaTimeInSec);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, updateInfo.DeltaTimeInSec);
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, updateInfo.DeltaTimeInSec);

    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
        frustumFollowsCamera = !frustumFollowsCamera;


}




