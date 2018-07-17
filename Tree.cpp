#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Tree.h"

using namespace std;

Tree::Tree(float x, float y, float z, float yRot, Shaders *shaders) {
    this->mX = x;
    this->mY = y;
    this->mZ = z;
    this->mYRot = yRot;
    this->mShaders = shaders;
}

void Tree::loadModel() {
    if (rand() % 2 == 0) {
        assertLoadModel("models/tree/PineTree03.obj");
    } else {
        assertLoadModel("models/pine/PineTransp.obj");
    }
    mProgramID = mShaders->loadModelShader();
}

void Tree::draw() {
    glUseProgram(mProgramID);

    glm::mat4 modelMatrix;
    modelMatrix = glm::translate(modelMatrix, glm::vec3(mX, mY, mZ));

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

        glDrawArrays(GL_TRIANGLES, 0, 3 * o.numTriangles);
    }
}

