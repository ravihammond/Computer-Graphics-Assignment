#include <stdio.h>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Skybox.h"

#include "stb_image.h"

#define VALS_PER_VERT 3

using namespace std;

// Constructor stores basic member variables
Skybox::Skybox() {}
Skybox::Skybox(Shaders *shaders) {
	this->mShaders = shaders;
}

// Deletes everything stored on Heap.
Skybox::~Skybox() {}

// Builds and loads all models in the scene.
void Skybox::loadModel() {
	// Generate VAO data.
	VAOVIData data = generateData();

	addVAOToGraphicsCard (
			&mVAOHandle, 
			1, 
			data.vertices, 
			data.vertices_size, 
			data.indices,
			data.indices_size);
	mIndicesSize = data.indices_size;

	mProgramID = mShaders->loadSkyboxShader();

	createTexture();
}

void Skybox::addVAOToGraphicsCard(
		unsigned int* vaoHandle, int vaoNumber, 
		float* vertices, int vertices_size, 
		unsigned int* indices, int indices_size) {

	glGenVertexArrays(vaoNumber, vaoHandle);
	glBindVertexArray(*vaoHandle);

	// Buffers to store position, colour and index data
	unsigned int buffer[2];
	glGenBuffers(2, buffer);

	// Set vertex position
	glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
	glBufferData(GL_ARRAY_BUFFER, 
			sizeof(float) * vertices_size, vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, VALS_PER_VERT, GL_FLOAT, GL_FALSE, 0, 0);

	// Set element attributes. Notice the change to using GL_ELEMENT_ARRAY_BUFFER
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
			sizeof(unsigned int) * indices_size, indices, GL_STATIC_DRAW);   

	// Un-bind
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

VAOVIData Skybox::generateData() {
	int vertices_size = 24;
	float* vertices = new float[vertices_size];
	int indices_size = 36;
	unsigned int *indices = new unsigned int[indices_size];

	vertices[0] = 1.0f;
	vertices[1] = 1.0f;
	vertices[2] = 1.0f;

	vertices[3] = -1.0f;
	vertices[4] = 1.0f;
	vertices[5] = 1.0f;

	vertices[6] = -1.0f;
	vertices[7] = -1.0f;
	vertices[8] = 1.0f;

	vertices[9] = 1.0f;
	vertices[10] = -1.0f;
	vertices[11] = 1.0f;

	vertices[12] = 1.0f;
	vertices[13] = 1.0f;
	vertices[14] = -1.0f;

	vertices[15] = -1.0f;
	vertices[16] = 1.0f;
	vertices[17] = -1.0f;

	vertices[18] = -1.0f;
	vertices[19] = -1.0f;
	vertices[20] = -1.0f;

	vertices[21] = 1.0f;
	vertices[22] = -1.0f;
	vertices[23] = -1.0f;

	indices[0] = 4;
	indices[1] = 0;
	indices[2] = 3;
	indices[3] = 3;
	indices[4] = 7;
	indices[5] = 4;

	indices[6] = 1;
	indices[7] = 5;
	indices[8] = 6;
	indices[9] = 6;
	indices[10] = 2;
	indices[11] = 1;

	indices[12] = 4;
	indices[13] = 5;
	indices[14] = 1;
	indices[15] = 1;
	indices[16] = 0;
	indices[17] = 4;

	indices[18] = 3;
	indices[19] = 2;
	indices[20] = 6;
	indices[21] = 6;
	indices[22] = 7;
	indices[23] = 3;

	indices[24] = 0;
	indices[25] = 1;
	indices[26] = 2;
	indices[27] = 2;
	indices[28] = 3;
	indices[29] = 0;

	indices[30] = 5;
	indices[31] = 4;
	indices[32] = 7;
	indices[33] = 7;
	indices[34] = 6;
	indices[35] = 5;

	VAOVIData data(vertices, vertices_size, 
			indices, indices_size);
	return data;
}

void Skybox::createTexture() {
	vector<string> faces;
    faces.push_back("skyboxes/DarkStormy/right.png");
    faces.push_back("skyboxes/DarkStormy/left.png");
    faces.push_back("skyboxes/DarkStormy/up.png");
    faces.push_back("skyboxes/DarkStormy/down.png");
    faces.push_back("skyboxes/DarkStormy/back.png");
    faces.push_back("skyboxes/DarkStormy/front.png");

	glGenTextures(1, &mTexID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mTexID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++) {
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
					0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
					);
			stbi_image_free(data);
		}
		else {
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE); 
}

// Renders all models in world
void Skybox::draw() {
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);

    glBindVertexArray(mVAOHandle);
    glUseProgram(mProgramID);
    mShaders->setSkyboxUniforms(mProgramID);

    glBindTexture(GL_TEXTURE_CUBE_MAP, mTexID);

    glDrawElements(GL_TRIANGLES, mIndicesSize, GL_UNSIGNED_INT, 0);

    glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);
}


