#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <iostream>
#include <thread>
#include <chrono>

#include "rendering/UpdateInfo.h"
#include "MyApp.h"

static constexpr int RED_BITS = 8;
static constexpr int GREEN_BITS = 8;
static constexpr int BLUE_BITS = 8;
static constexpr int ALPHA_BITS = 8;
static constexpr int DEPTH_BITS = 24;

static constexpr bool ENABLE_ANTIALIASING = false;
static constexpr int ANTIALIASING_LEVEL = 4;

static constexpr bool ENABLE_VSYNC = false;
static int targetFPS = 300;

// > sudo intel_gpu_top

static constexpr int WINDOW_WIDTH = 1280;
static constexpr int WINDOW_HEIGHT = 720;
static auto WINDOW_TITLE = "OpenGL";

static bool enableBoundingBoxRenderer = false;


// Callback to adjust the viewport when the user resizes the window
static void framebuffer_size_callback(GLFWwindow* window, const int width, const int height) {
    glViewport(0, 0, width, height);
}

// key :: keycode
// action :: 0 - released, 1 - pressed, 2 - held
// mods :: shift(1) + ctrl(2) + alt(4)
static void processInput(GLFWwindow* window, const int key, int scancode, const int action, int mods) {
    static bool wireFrameMode = false;
    static bool mouseHidden = false;

    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    else if ( key == GLFW_KEY_F2 && action == GLFW_PRESS ){
        if (!wireFrameMode)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        wireFrameMode = !wireFrameMode;
    }
    // Hides the cursor and locks it to the window
    else if (key == GLFW_KEY_F1 && action == GLFW_PRESS) {
        if (!mouseHidden)
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        else
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        mouseHidden = !mouseHidden;
    }
    else if (key == GLFW_KEY_B && action == GLFW_PRESS) {
        enableBoundingBoxRenderer = !enableBoundingBoxRenderer;
    }


    //std::cout << "Key:" << key << ", code:" << scancode << ", action:" << action << ", mods:" << mods << std::endl;
}

int main() {
    // 1. Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Configure GLFW for OpenGL 3.3 Core Profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // RGBA színkomponensek mérete (8-8-8-8 bit -> 32 bites buffer)
    glfwWindowHint(GLFW_RED_BITS, RED_BITS);
    glfwWindowHint(GLFW_GREEN_BITS, GREEN_BITS);
    glfwWindowHint(GLFW_BLUE_BITS, BLUE_BITS);
    glfwWindowHint(GLFW_ALPHA_BITS, ALPHA_BITS);

    // Duplapufferelés (GLFW-ben az OpenGL ablakok alapértelmezetten duplapuffereltek,
    // de explicit megadható a doublebuffer hint-tel)
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

    // Mélységi puffer mérete (24 bit)
    glfwWindowHint(GLFW_DEPTH_BITS, DEPTH_BITS);

    // Antialiasing / MSAA - ha kell
    if (ENABLE_ANTIALIASING)
        glfwWindowHint(GLFW_SAMPLES, ANTIALIASING_LEVEL);


    // 2. Create Window
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Toggle VSync
    glfwSwapInterval(ENABLE_VSYNC ? 1 : 0);

    // 3. Initialize GLAD
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // When we're talking specifically about the vertex shader each input variable is also known as a vertex attribute.
    // There is a maximum number of vertex attributes we're allowed to declare limited by the hardware.
    // OpenGL guarantees there are always at least 16 4-component vertex attributes available,
    // but some hardware may allow for more which you can retrieve by querying GL_MAX_VERTEX_ATTRIBS:
    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

    int glVersion[2] = {-1, -1};
    glGetIntegerv(GL_MAJOR_VERSION, &glVersion[0]);
    glGetIntegerv(GL_MINOR_VERSION, &glVersion[1]);

    // kérdezzük le a használt GPU nevét...
    const char* renderer     = reinterpret_cast<const char*>(glGetString( GL_RENDERER ));
    // ...és a GLSL verziót
    const char* glsl_version = reinterpret_cast<const char*>(glGetString( GL_SHADING_LANGUAGE_VERSION ));

    std::cout << "Running OpenGL "<< glVersion[0] << "." << glVersion[1] << std::endl;
    std::cout << "Renderer: " << renderer << std::endl;
    std::cout << "GLSL version:" << glsl_version << std::endl;


    // 4. Setup Dear ImGui Context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& im_gui_io = ImGui::GetIO(); (void)im_gui_io;
    im_gui_io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls

    ImGui::StyleColorsDark(); // Setup ImGui style

    // Setup Platform/Renderer bindings
    //constexpr auto glsl_version = "#version 330";
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(nullptr); // imgui picks the glsl version automatically

    {
        MyApp my_app;

        glfwSetWindowUserPointer(window, &my_app);
        // Mouse position callback
        glfwSetCursorPosCallback(window, [](GLFWwindow* win, const double xPos, const double yPos) {
            if (auto* app = static_cast<MyApp*>(glfwGetWindowUserPointer(win))) {
                app->MouseClickEvent(win, xPos, yPos);
            }
        });

        // Mouse Scroll callback
        glfwSetScrollCallback(window, [](GLFWwindow* win, const double xOffset, const double yOffset) {
            if (auto* app = static_cast<MyApp*>(glfwGetWindowUserPointer(win))) {
                app->MouseScrollEvent(win, xOffset, yOffset);
            }
        });

        bool quit = !my_app.Init();

        double frameStartTime = glfwGetTime();
        // 5. Main Render Loop
        while (!quit) {

            // input
            glfwSetKeyCallback(window, processInput);

            // --- Application GUI ---

            int display_w, display_h;
            glfwGetFramebufferSize(window, &display_w, &display_h);
            glViewport(0, 0, display_w, display_h);


            // Delta time
            const double currentTime = glfwGetTime(); // Másodpercben adja vissza (double)
            SUpdateInfo updateInfo
            {
                .ElapsedTimeInSec = static_cast<float>(currentTime),            // Eltelt összidő másodpercben
                .DeltaTimeInSec = static_cast<float>(currentTime - frameStartTime)    // Delta time (dt) másodpercben
            };
            frameStartTime = currentTime;

            // Events
            my_app.SetResolution(display_w, display_h);
            my_app.KeyEvent(window, updateInfo);
            my_app.Update(updateInfo);
            my_app.enableBoundingBoxRenderer = enableBoundingBoxRenderer;
            my_app.Render();

            // Start the ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            my_app.RenderGUI(im_gui_io);

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            // Poll and handle events (inputs, window resize, etc.)
            glfwPollEvents();

            // Swap buffers
            glfwSwapBuffers(window);

            // 2. Throttle the frame rate
            if (targetFPS > 0) {
                const double targetFrameDuration = 1.0 / static_cast<double>(targetFPS);
                double elapsedTime = glfwGetTime() - frameStartTime;

                // Sleep if we have enough headroom (leave ~1-2ms for precise spin-wait)
                while (elapsedTime < targetFrameDuration) {
                    // Sleep the thread for the bulk of the wait to save CPU usage
                    if (const double remaining = targetFrameDuration - elapsedTime; remaining > 0.002) {
                        std::this_thread::sleep_for(std::chrono::milliseconds(1));
                    }
                    // Spin-lock the final fraction of a millisecond for timing precision
                    elapsedTime = glfwGetTime() - frameStartTime;
                }
            }


            if (glfwWindowShouldClose(window))
                quit = true;
        }
        my_app.Clean();
    }

    // 6. Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
