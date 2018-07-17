#include <stdio.h>
#include <math.h>
#include <utility>
#include <vector>

#include "Player.h"

#define ACCELERATION 0.005f
#define DECELERATION 0.01f
#define VELOCITY_MAX 0.28f
#define VELOCITY_MIN -0.1f

#define ANG_ACCELERATION 0.002f
#define ANG_DECELERATION 0.002f
#define ANG_VELOCITY_MAX 0.02f
#define ANG_VELOCITY_MIN -0.02f

using namespace std;

Player::Player() : 
    mX(0.0f), 
    mY(0.0f), 
    mZ(0.0f), 
    mYRot(0.0f), 
    mCameraState(true),
    mVelocity(0.0f),
    mAngVelocity(0.0f) {
}

void Player::update(InputState *input) {
    bool up = input->upPressed;
    bool down = input->downPressed;
    bool left = input->leftPressed;
    bool right = input->rightPressed;

    if (up && !down) {
        float newVelocity = mVelocity + ACCELERATION;
        mVelocity = newVelocity > VELOCITY_MAX ? VELOCITY_MAX : newVelocity;
    }

    else if (!up && down) {
        float newVelocity = mVelocity - ACCELERATION;
        mVelocity = newVelocity < VELOCITY_MIN ? VELOCITY_MIN : newVelocity;
    }

    else {
        if (mVelocity > 0.0f) {
            float newVelocity = mVelocity - DECELERATION;
            mVelocity = newVelocity > 0.0f ? newVelocity : 0.0f;
        } else if (mVelocity < 0.0f) {
            float newVelocity = mVelocity + DECELERATION;
            mVelocity = newVelocity < 0.0f ? newVelocity : 0.0f;
        }
    }

    if (left && !right) {
        float newAngVelocity = mAngVelocity + ANG_ACCELERATION;
        mAngVelocity = newAngVelocity > ANG_VELOCITY_MAX ? 
            ANG_VELOCITY_MAX : newAngVelocity;
    }

    else if (!left && right) {
        float newAngVelocity = mAngVelocity - ANG_ACCELERATION;
        mAngVelocity = newAngVelocity < ANG_VELOCITY_MIN ? 
            ANG_VELOCITY_MIN : newAngVelocity;
    }

    else {
        if (mAngVelocity > 0.0f) {
            float newAngVelocity = mAngVelocity - ANG_DECELERATION;
            mAngVelocity = newAngVelocity > 0.0f ? newAngVelocity : 0.0f;
        } else if (mAngVelocity < 0.0f) {
            float newAngVelocity = mAngVelocity + ANG_DECELERATION;
            mAngVelocity = newAngVelocity < 0.0f ? newAngVelocity : 0.0f;
        }
    }


    if (mVelocity < 0) {
        mYRot -= mAngVelocity;
    } 
    if (mVelocity > 0){
        mYRot += mAngVelocity;
    }

    float newX = mX + (mVelocity * sin(mYRot));
    float newZ = mZ + (mVelocity * cos(mYRot));

    if (collision(newX, newZ)) {
        mVelocity = 0.0f;
        mAngVelocity = 0.0f;
    } else {
        mX += mVelocity * sin(mYRot);
        mZ += mVelocity * cos(mYRot);
    }
}

bool Player::collision(float x, float z) {
    float frontX = x + (1.1 * sin(mYRot));
    float frontZ = z + (1.1 * cos(mYRot));
    float backX = x - (1.1 * sin(mYRot));
    float backZ = z - (1.1 * cos(mYRot));

    if (edgeCollision(frontX, frontZ) ||
        edgeCollision(backX, backZ)) {
        return true;
    }

    if (treeCollision(x, z)) {
        return true;
    }

    return false;
}

bool Player::edgeCollision(float x, float z) {
    if (x > 50 || x < -50 || z > 50 || z < -50) {
        return true;
    }
    return false;
}

bool Player::treeCollision(float x, float z) {
    glm::mat4 reverseCarMatrix;
    reverseCarMatrix = glm::scale(reverseCarMatrix, 
            glm::vec3(1 / 0.6f, 1 / 0.6f, 1 / 0.6f));
    reverseCarMatrix = glm::rotate(reverseCarMatrix, 
            mYRot + float(M_PI), glm::vec3(0.0f, -1.0f, 0.0f));
    reverseCarMatrix = 
        glm::translate(reverseCarMatrix, glm::vec3(x, 0.0f, z));


    float carX = 1.0f;
    float carZ = 2.0f;

    for (auto coord : mTreeCoords) {
        glm::vec4 treeCollisionPosition = reverseCarMatrix * 
            glm::vec4(coord.first, 0.0f, coord.second, 1.0f);

        float treeX = treeCollisionPosition.x;
        float treeZ = treeCollisionPosition.z;

        if (treeX > -1 * carX && treeX < carX &&
            treeZ > -1 * carZ && treeZ < carZ) {
            return true;
        }
    }

    return false;
}

void Player::toggleCameraState() {
    mCameraState = !mCameraState; 
}

float Player::getX() {
    return mX;
}

float Player::getY() {
    return mY;
}

float Player::getZ() {
    return mZ;
}

float Player::getYRot() {
    return mYRot;
}

bool Player::getCameraState() {
    return mCameraState;
}

void Player::setTreeCoords(vector<pair<float,float>> treeCoords) {
    mTreeCoords = treeCoords;
}

