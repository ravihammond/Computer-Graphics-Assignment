#include <stdio.h>
#include <math.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Car.h"

using namespace std;

Car::Car(float x, float y, float z, float yRot, Shaders *shaders, Player *player) {
    this->mX = x;
    this->mY = y;
    this->mZ = z;
    this->mYRot = yRot;
    this->mShaders = shaders;
    this->mPlayer = player;
}

void Car::loadModel() {
    assertLoadModel("models/car/car-n.obj");
    mProgramID = mShaders->loadModelShader();
}

void Car::draw() {
    this->mX = mPlayer->getX();
    this->mZ = mPlayer->getZ();
    this->mYRot = mPlayer->getYRot();

    glUseProgram(mProgramID);

    glm::mat4 modelMatrix;
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-1 * mX, mY, -1 * mZ));
    modelMatrix = glm::rotate(modelMatrix, mYRot + float(M_PI), glm::vec3(0.0f, 1.0f, 0.0f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.6f, 0.6f, 0.6f));

    //modelMatrix = glm::scale(modelMatrix, glm::vec3(1 / 0.6f, 1 / 0.6f, 1 / 0.6f));
    //modelMatrix = glm::rotate(modelMatrix, mYRot + float(M_PI), glm::vec3(0.0f, -1.0f, 0.0f));
    //modelMatrix = glm::translate(modelMatrix, glm::vec3(mX, 0.0f, mZ));

    for (size_t i = 0; i < mDrawObjects.size(); i++) {
        // Extract model from vector.
        DrawObject o = mDrawObjects[i];
        // Extract material from model.
        tinyobj::material_t material = mMaterials[o.material_id];

        // Bind current model's VAO.
        glBindVertexArray(o.vao);

        // If model has a texture
        if (mTextures.size() > 0) {
            GLuint texture_id = mTextures[material.diffuse_texname];
            glBindTexture(GL_TEXTURE_2D, texture_id);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);	
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);	
        }

        mShaders->setModelUniforms(mProgramID, 
                modelMatrix,
                glm::make_vec3(material.ambient),
                glm::make_vec3(material.diffuse),
                glm::make_vec3(material.specular),
                material.shininess,
                0,
                mTextures.size() > 0 ? 1 : 0);

        if (mPlayer->getCameraState()) {
            glDrawArrays(GL_TRIANGLES, 0, 3 * o.numTriangles);
        }
    }
}


