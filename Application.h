#ifndef _APPLICATION_H
#define _APPLICATION_H

#include <GLFW/glfw3.h>

#include "World.h"
#include "Shaders.h"
#include "Player.h"

// Uses A* algorithm to find the shortest path in a 2D kk
class Application {
    public:
        // Constructor stores basic member variables
        Application();
        // Deletes everything stored on Heap.
        ~Application();
        // Initialised view vectors
        void run();
        // Runs the render Loop.
        void render();

        // Handles shader creation and shader switching
        Shaders* mShaders;
        // Controls the location of the player, camera state, velocity
        Player* mPlayer;
        // Data structure storing keyboard input info
        InputState mInput;
        
    private:
        // Holds the opengl window.
        GLFWwindow* mWindow;
        // World that contains all models
        World mWorld;

};

#endif

