#ifndef _SHADERS_H
#define _SHADERS_H

#include <vector>
#include <string>

#include "glm/glm.hpp"
#include "Player.h"

// World that contains all models in the scene
class Shaders {
    public:
        // Constructor stores basic member variables
        Shaders(int* width, int* height, Player* player);
        // Deletes everything stored on Heap.
        ~Shaders();
        // Load ground shaders.
        unsigned int loadGroundShader();
        // Load skybox shaders.
        unsigned int loadSkyboxShader();
        // Load model shaders.
        unsigned int loadModelShader();
        // Sets projections for all shaders
        void setAllProjections();
        // Set uniforms for ground
        void setGroundUniforms(unsigned int programID);
        // Set uniforms for skybox
        void setSkyboxUniforms(unsigned int programID);
        // Set uniforms for model
        void setModelUniforms(unsigned int programID, 
                glm::mat4 modelMatrix,
                glm::vec3 mtl_ambient,
                glm::vec3 mtl_diffuse,
                glm::vec3 mtl_specular,
                float shininess,
                int texMap,
                int texExists);

    private:
        void setDirectionalLightUniforms(unsigned int programID);
        void setPointLightUniforms(unsigned int programID);
        // Set the view uniform matrix
        void setViewMatrixUniform(unsigned int programID, glm::mat4 getMatrix);
        // Create view Matrix
        glm::mat4 getViewMatrix();
        // Create view Matrix
        glm::mat4 getRotationMatrix();
        // Checks to make sure both shaders loaded correctly
        void assertShaders(unsigned int programID, std::string name);
        // Checks to make sure uniform exists in shader
        void assertUniform(int handle, std::string name);
        // Sets projection for a single shader.
        void setProjection(unsigned int programID);

        // All program ID's
        std::vector<unsigned int> mAllShaders;
        // Screen dimensions
        int* mWidth;
        int* mHeight;
        Player* mPlayer;

        unsigned int mModelShaderIndex;
         
};

#endif


