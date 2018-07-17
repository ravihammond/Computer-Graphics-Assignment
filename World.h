#ifndef _WORLD_H
#define _WORLD_H

#include <vector>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Ground.h"
#include "Skybox.h"
#include "Shaders.h"
#include "Model.h"
#include "Car.h"

// World that contains all models in the scene
class World {
    public:
        // Constructor stores basic member variables
        World();
        World(Shaders* shaders, Player* player);
        // Deletes everything stored on Heap.
        ~World();
        // Builds and loads all models in the scene.
        void buildModels();
        // Renders all models in world
        void render();

    private:
        std::vector<std::pair<float,float>>  genTreeLocations();
        // Ground of the world
        Ground mGround;
        // Skybox showing the sky
        Skybox mSkybox;
        Car* mCar;
        std::vector<Model*> mModels;
        // Handles shader creation and shader switching
        Shaders* mShaders;
};

#endif

