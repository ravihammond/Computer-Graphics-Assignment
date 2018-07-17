#ifndef _PLAYER_H
#define _PLAYER_H

#include <utility>
#include <vector>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "InputState.h"

class Player {
    public:
        Player();
        void update(InputState *input);
        void toggleCameraState();
        float getX();
        float getY();
        float getZ();
        float getYRot();
        bool getCameraState();
        void setTreeCoords(std::vector<std::pair<float,float>> treeCoords);

    private:
        bool collision(float x, float y);
        bool edgeCollision(float x, float y);
        bool treeCollision(float x, float y);
        float mX;
        float mY;
        float mZ;
        float mYRot;
        bool mCameraState;
        float mVelocity;
        float mAngVelocity;
        std::vector<std::pair<float,float>> mTreeCoords;
};

#endif


