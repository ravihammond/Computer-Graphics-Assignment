#define GLFW_INCLUDE_NONE

#include <iostream>
#include <stdlib.h>
#include <cstdlib> 

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "InputState.h"
#include "Player.h"
#include "Application.h"

// All callback functions.
void error_callback(int error, const char* description);
void reshape_callback(GLFWwindow* window, int w, int h);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

// Dimensions of the screen.
int width = 640;
int height = 640;

// State of the Camera
int cameraState = 0;

using namespace std;

// Constructor and Destructor don't get used.
Application::Application() {
    std::cout << "Controls: mouse left-click + drag rotates camera around model(s)\n";
    std::cout << "          mouse right-click + drag zooms camera in and out\n";
    std::cout << "          d toggles between debug_inspect mode and lighting mode\n";
    std::cout << "          b cycles debug modes: polygon, normal colour, and material diffuse\n";
    std::cout << "          s cycles lighting modes: overhead directional, camera torch, rotating light, and no light\n";

    mPlayer = new Player();
    mShaders = new Shaders(&width, &height, mPlayer);
    mWorld = World(mShaders, mPlayer);
}

Application::~Application() {}

// Program entry. Sets up OpenGL state, GLSL Shaders and window and function call backs
// Takes no arguments
void Application::run() {
    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
        exit(1);
    }

    // Specify that we want OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create the window and OpenGL context
    mWindow = glfwCreateWindow(width, height, "Forest Drive", NULL, NULL);
    if (!mWindow) {
        std::cerr << "Failed to open GLFW window. " << std::endl;
        glfwTerminate();
        exit(1);
    }

    glfwMakeContextCurrent(mWindow);
    glfwSwapInterval(1);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW." << std::endl;
        exit(1);
    }

    // Initialise OpenGL state
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    mWorld.buildModels();

    // Define callback functions and start main loop
    glfwSetWindowUserPointer(mWindow, this);
    glfwSetKeyCallback(mWindow, key_callback);
    glfwSetFramebufferSizeCallback(mWindow, reshape_callback);

    // Initialise projection, shader states, and size of screen.
    mShaders->setAllProjections();
    reshape_callback(mWindow, width, height);

    //render loop
    while (!glfwWindowShouldClose(mWindow)) {
        render();

        glfwSwapBuffers(mWindow);
        glfwPollEvents();
    }

    // Clean up
    glfwDestroyWindow(mWindow);
    glfwTerminate();
    exit(0);
}   

// Renders a frame using the shaders and data we've set up.
void Application::render() {
    glfwPollEvents();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    mPlayer->update(&mInput);
    mWorld.render();

    glBindVertexArray(0);

    glFlush();
}

// Changes the perspective whenever the window resizes.
void reshape_callback(GLFWwindow* window, int w, int h) {
    Application * app = reinterpret_cast<Application *>(glfwGetWindowUserPointer(window));

    width = w;
    height = h;

    //Set projection for all program ID's
    app->mShaders->setAllProjections();

    glViewport( 0, 0, w, h );
}

// Called while a keyboard key is pressed.
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Application * app = reinterpret_cast<Application *>(glfwGetWindowUserPointer(window));

    if (action == GLFW_PRESS) {
        switch(key) {
            case GLFW_KEY_ESCAPE: 
                glfwSetWindowShouldClose(window, GL_TRUE);
                break;
            case GLFW_KEY_SPACE:
                app->mPlayer->toggleCameraState();
                break;
            case GLFW_KEY_UP:
                app->mInput.upPressed = true;
                break;
            case GLFW_KEY_DOWN:
                app->mInput.downPressed = true;
                break;
            case GLFW_KEY_LEFT:
                app->mInput.leftPressed = true;
                break;
            case GLFW_KEY_RIGHT:
                app->mInput.rightPressed = true;
                break;
            case GLFW_KEY_S:
                break;
            default:
                break;
        }
    }

    if (action == GLFW_RELEASE) {
        switch(key) {
            case GLFW_KEY_UP:
                app->mInput.upPressed = false;
                break;
            case GLFW_KEY_DOWN:
                app->mInput.downPressed = false;
                break;
            case GLFW_KEY_LEFT:
                app->mInput.leftPressed = false;
                break;
            case GLFW_KEY_RIGHT:
                app->mInput.rightPressed = false;
                break;
            default:
                break;
        }
    }
}

// Error callback for GLFW. Simply prints error message to stderr.
void error_callback(int error, const char* description) {
    std::cerr << description;
}
