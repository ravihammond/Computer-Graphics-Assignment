#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Shaders.h"
#include "shader.hpp"

using namespace std;

// Constructor stores basic member variables
Shaders::Shaders(int* width, int* height, Player* player) {
    this->mWidth = width;
    this->mHeight = height;
    this->mPlayer = player;
    mAllShaders = vector<unsigned int>();
    mModelShaderIndex = UINT_MAX;
}

// Deletes everything stored on Heap.
Shaders::~Shaders() {}

// Load ground shader.
unsigned int Shaders::loadGroundShader() {
    unsigned int programID = LoadShaders("ground.vert", "ground.frag");
    assertShaders(programID, "Ground");

    mAllShaders.push_back(programID);
    return programID;
}

// Load skybox shader.
unsigned int Shaders::loadSkyboxShader() {
    unsigned int programID = LoadShaders("skybox.vert", "skybox.frag");
    assertShaders(programID, "Skybox");

    mAllShaders.push_back(programID);
    return programID;
}

// Load skybox shader.
unsigned int Shaders::loadModelShader() {
    if (mModelShaderIndex < UINT_MAX) {
        return mAllShaders[mModelShaderIndex];
    }

    unsigned int programID = LoadShaders("model.vert", "model.frag");
    assertShaders(programID, "Model");

    mAllShaders.push_back(programID);
    mModelShaderIndex = mAllShaders.size() - 1;

    return programID;
}

// Creates perspective matrix and creates the correct uniform variable.
void Shaders::setProjection(unsigned int programID) {
    glm::mat4 projection;
    projection = glm::perspective( (float)M_PI/3.0f, (float) *mWidth / *mHeight, 0.1f, 100.0f );

    // Load it to the shader program
    string projName = "projection_matrix";
    int projHandle = glGetUniformLocation(programID, projName.c_str());
    assertUniform(projHandle, projName);

    glUseProgram(programID);
    glUniformMatrix4fv( projHandle, 1, false, glm::value_ptr(projection) );
}

void Shaders::setAllProjections() {
    for (auto programID : mAllShaders) {
        setProjection(programID);
    }
}

void Shaders::setSkyboxUniforms(unsigned int programID) {
    glUseProgram(programID);

    glm::mat4 viewMatrix = glm::mat4(glm::mat3(getViewMatrix()));  
    setViewMatrixUniform(programID, viewMatrix);
}

void Shaders::setGroundUniforms(unsigned int programID) {
    glUseProgram(programID);
    setDirectionalLightUniforms(programID);
    setPointLightUniforms(programID);
    setViewMatrixUniform(programID, getViewMatrix());

    string texName = "texMap";
    int texHandle = glGetUniformLocation(programID, texName.c_str());
    assertUniform(texHandle, texName);
    glUniform1i(texHandle, 0);
}

void Shaders::setModelUniforms(unsigned int programID, 
        glm::mat4 modelMatrix,
        glm::vec3 mtl_ambient,
        glm::vec3 mtl_diffuse,
        glm::vec3 mtl_specular,
        float shininess,
        int texMap,
        int texExists) {

    glUseProgram(programID);
    setDirectionalLightUniforms(programID);
    setPointLightUniforms(programID);
    setViewMatrixUniform(programID, getViewMatrix());

    string modelName = "model_matrix";
    string ambName = "mtl_ambient";
    string diffName = "mtl_diffuse";
    string specName = "mtl_specular";
    string shineName = "shininess";
    string texName = "texMap";
    string texExistsName = "texture_exists";

    int ambHandle = glGetUniformLocation(programID, ambName.c_str());
    int modelHandle = glGetUniformLocation(programID, modelName.c_str());
    int diffHandle = glGetUniformLocation(programID, diffName.c_str());
    int specHandle = glGetUniformLocation(programID, specName.c_str());
    int shineHandle = glGetUniformLocation(programID, shineName.c_str());
    int texHandle = glGetUniformLocation(programID, texName.c_str());
    int texExistsHandle = glGetUniformLocation(programID, texExistsName.c_str());

    assertUniform(modelHandle, modelName);
    assertUniform(ambHandle, ambName);
    assertUniform(diffHandle, diffName);
    assertUniform(specHandle, specName);
    assertUniform(shineHandle, shineName);
    assertUniform(texHandle, texName);
    assertUniform(texExistsHandle, texExistsName);

    glUniformMatrix4fv(modelHandle, 1, false, glm::value_ptr(modelMatrix));
    glUniform3fv(ambHandle, 1, glm::value_ptr(mtl_ambient));
    glUniform3fv(diffHandle, 1, glm::value_ptr(mtl_diffuse));
    glUniform3fv(specHandle, 1, glm::value_ptr(mtl_specular));
    glUniform1f(shineHandle, shininess);
    glUniform1i(texHandle, texMap);
    glUniform1i(texExistsHandle, texExists);
}

void Shaders::setDirectionalLightUniforms(unsigned int programID) {
    glm::vec3 directionalAmbient = glm::vec3(0.1f, 0.1f, 0.1f);
    glm::vec3 directionalDiffuse = glm::vec3(0.8f, 0.8f, 0.8f);
    glm::vec3 directionalSpecular = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::vec3 lightDirection = glm::vec3(0.0f, 1.0f, 1.0f);

    string ambName = "directional_ambient";
    string diffName = "directional_diffuse";
    string specName = "directional_specular";
    string dirName = "directional_light_direction";

    int ambHandle = glGetUniformLocation(programID, ambName.c_str());
    int diffHandle = glGetUniformLocation(programID, diffName.c_str());
    int specHandle = glGetUniformLocation(programID, specName.c_str());
    int dirHandle = glGetUniformLocation(programID, dirName.c_str());

    //assertUniform(ambHandle, ambName);
    //assertUniform(diffHandle, diffName);
    //assertUniform(specHandle, specName);
    //assertUniform(dirHandle, dirName);

    glUniform3fv(ambHandle, 1, glm::value_ptr(directionalAmbient));
    glUniform3fv(diffHandle, 1, glm::value_ptr(directionalDiffuse));
    glUniform3fv(specHandle, 1, glm::value_ptr(directionalSpecular));
    glUniform3fv(dirHandle, 1, glm::value_ptr(lightDirection));
}

void Shaders::setPointLightUniforms(unsigned int programID) {
    glm::vec3 pointAmbient = glm::vec3(0.1f, 0.1f, 0.1f);
    glm::vec3 pointDiffuse = glm::vec3(0.1f, 0.1f, 1.0f);
    glm::vec3 pointSpecular = glm::vec3(0.1f, 0.1f, 1.0f);
    glm::vec4 pointPosition = glm::vec4(0.0f, 2.0f, 0.0f, 1.0f);

    string ambName = "point_ambient";
    string diffName = "point_diffuse";
    string specName = "point_specular";
    string posName = "point_light_position";

    int ambHandle = glGetUniformLocation(programID, ambName.c_str());
    int diffHandle = glGetUniformLocation(programID, diffName.c_str());
    int specHandle = glGetUniformLocation(programID, specName.c_str());
    int posHandle = glGetUniformLocation(programID, posName.c_str());

    //assertUniform(ambHandle, ambName);
    //assertUniform(diffHandle, diffName);
    //assertUniform(specHandle, specName);
    //assertUniform(posHandle, posName);

    glUniform3fv(ambHandle, 1, glm::value_ptr(pointAmbient));
    glUniform3fv(diffHandle, 1, glm::value_ptr(pointDiffuse));
    glUniform3fv(specHandle, 1, glm::value_ptr(pointSpecular));
    glUniform4fv(posHandle, 1, glm::value_ptr(pointPosition));
}

glm::mat4 Shaders::getViewMatrix() {
    glm::mat4 viewMatrix;

    if (mPlayer->getCameraState()) {
        viewMatrix = glm::translate(viewMatrix, glm::vec3(0.0f, -1.5f, -6.0f));
        viewMatrix = glm::rotate(viewMatrix, 50.0f, glm::vec3(-1.0f, 0.0f, 0.0f));
        viewMatrix = glm::rotate(viewMatrix, mPlayer->getYRot(), glm::vec3(0.0f, -1.0f, 0.0f));
        viewMatrix = glm::translate(viewMatrix, glm::vec3(mPlayer->getX(), -1.0f, mPlayer->getZ()));
    }
    else {
        viewMatrix = glm::rotate(viewMatrix, mPlayer->getYRot(), glm::vec3(0.0f, -1.0f, 0.0f));
        viewMatrix = glm::translate(viewMatrix, glm::vec3(mPlayer->getX(), -1.0f, mPlayer->getZ()));
    }

    return viewMatrix;
}

void Shaders::setViewMatrixUniform(unsigned int programID, glm::mat4 viewMatrix) {
    glUseProgram(programID);

    string viewName = "view_matrix";
    int viewHandle = glGetUniformLocation(programID, viewName.c_str());
    assertUniform(viewHandle, viewName);
    glUniformMatrix4fv(viewHandle, 1, false, glm::value_ptr(viewMatrix));
}

void Shaders::assertShaders(unsigned int programID, string name) {
    if (programID == 0) {
        fprintf(stderr, "Can't compile shaders for: %s\n", name.c_str());
        exit(1);
    }    
}

void Shaders::assertUniform(int handle, string name) {
    if (handle == -1) { 
        fprintf(stderr, "Uniform: %s is not an active uniform label\n", name.c_str());
        exit(1);
    }
}

