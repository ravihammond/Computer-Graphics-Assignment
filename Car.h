#ifndef _CAR_H
#define _CAR_H

#include "Model.h"
#include "Player.h"

class Car: public Model {
    public:
        Car(float x, float y, float z, float yRot, Shaders* shaders, Player* player);
        // Builds and loads all models in the scene.
        void loadModel();
        // Renders all models in world
        void draw();
    private:
        Player* mPlayer;
};

#endif


