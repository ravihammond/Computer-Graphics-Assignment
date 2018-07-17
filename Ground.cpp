#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Ground.h"

#include "stb_image.h"

#define VALS_PER_VERT 3
#define VALS_PER_TEX 2

using namespace std;

// Constructor stores basic member variables
Ground::Ground() {}
Ground::Ground(Shaders *shaders) {
    this->mShaders = shaders;
}

// Deletes everything stored on Heap.
Ground::~Ground() {}

// Builds and loads all models in the scene.
void Ground::loadModel() {
    // Generate VAO data.
    VAOData data = generateData();

    addVAOToGraphicsCard (
            &mVAOHandle, 
            1, 
            data.vertices, 
            data.vertices_size, 
            data.indices,
            data.indices_size,
            data.normals,
            data.normals_size,
            data.tex_coords,
            data.tex_coords_size
            );
    mIndicesSize = data.indices_size;
    
    mProgramID = mShaders->loadGroundShader();

    createTexture();
}

void Ground::addVAOToGraphicsCard(
        unsigned int* vaoHandle, int vaoNumber, 
        float* vertices, int vertices_size, 
        unsigned int* indices, int indices_size, 
        float* normals, int normals_size,
        float* tex_coords, int tex_coords_size) {

    glGenVertexArrays(vaoNumber, vaoHandle);
    glBindVertexArray(*vaoHandle);

    // Buffers to store position, colour and index data
    unsigned int buffer[4];
    glGenBuffers(4, buffer);

    // Set vertex position
    glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
    glBufferData(GL_ARRAY_BUFFER, 
            sizeof(float) * vertices_size, vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, VALS_PER_VERT, GL_FLOAT, GL_FALSE, 0, 0);

    // Set normal position
    glBindBuffer(GL_ARRAY_BUFFER, buffer[1]);
    glBufferData(GL_ARRAY_BUFFER, 
            sizeof(float) * normals_size, normals, GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, VALS_PER_VERT, GL_FLOAT, GL_FALSE, 0, 0);

    // Texture attributes
    glBindBuffer(GL_ARRAY_BUFFER, buffer[2]);
    glBufferData(GL_ARRAY_BUFFER, 
            sizeof(float) * tex_coords_size, tex_coords, GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, VALS_PER_TEX, GL_FLOAT, GL_FALSE, 0, 0);

    // Set element attributes. Notice the change to using GL_ELEMENT_ARRAY_BUFFER
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[3]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
            sizeof(unsigned int) * indices_size, indices, GL_STATIC_DRAW);   

    // Un-bind
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

VAOData Ground::generateData() {
    float scale = 50.0f;

    int vertices_size = 12;
    float* vertices = new float[vertices_size];
    int normals_size = 12;
    float* normals = new float[normals_size];
    int tex_coords_size = 8;
    float* tex_coords = new float[tex_coords_size];
    int indices_size = 6;
    unsigned int *indices = new unsigned int[indices_size];

    vertices[0] = scale;
    vertices[1] = 0.0f;
    vertices[2] = scale;
    vertices[3] = -1 * scale;
    vertices[4] = 0.0f;
    vertices[5] = scale;

    vertices[6] = -1 * scale;
    vertices[7] = 0.0f;
    vertices[8] = -1 * scale;
    vertices[9] = scale;
    vertices[10] = 0.0f;
    vertices[11] = -1 * scale;

    for (int i = 0; i < 4; i++) {
        normals[(i * 3)] = 0.0f;
        normals[(i * 3) + 1] = 1.0f;
        normals[(i * 3) + 2] = 0.0f;
    }

    tex_coords[0] = 0.0f;
    tex_coords[1] = 0.0f;
    tex_coords[2] = scale;
    tex_coords[3] = 0.0f;
    tex_coords[4] = scale;
    tex_coords[5] = scale;
    tex_coords[6] = 0.0f;
    tex_coords[7] = scale;

    indices[0] = 0;
    indices[1] = 1;
    indices[2] = 2;
    indices[3] = 2;
    indices[4] = 3;
    indices[5] = 0;

    VAOData data(vertices, vertices_size, 
            indices, indices_size, 
            normals, normals_size,
            tex_coords, tex_coords_size);
    return data;
}

void Ground::createTexture() {
    glActiveTexture(GL_TEXTURE0);

    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

    glGenTextures(1, &mTexID);
    glBindTexture(GL_TEXTURE_2D, mTexID);

    // load an image from file as texture 1
    int x, y, n;
    unsigned char *data = stbi_load("grass/tilable-IMG_0044-verydark.png", &x, &y, &n, 0);

    if (n == 3) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    }
    else {
        fprintf(stderr, "Image pixels are not RGB. Texture may not load correctly.");
    }

    stbi_image_free(data);

    glGenerateMipmap(GL_TEXTURE_2D); 
}

// Renders all models in world
void Ground::draw() {
    glUseProgram(mProgramID);
    glBindVertexArray(mVAOHandle);

    mShaders->setGroundUniforms(mProgramID);

    glBindTexture(GL_TEXTURE_2D, mTexID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);	

    glDrawElements(GL_TRIANGLES, mIndicesSize, GL_UNSIGNED_INT, 0);
}

