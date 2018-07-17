#ifndef _SKYBOX_H
#define _SKYBOX_H

#include "Shaders.h"

// Struct to deal with passing the generates arrays back as one return statement to be
// added the to graphics card for open gel to use in the pipeline
struct VAOVIData { 
    float* vertices;
    int vertices_size;
    unsigned int* indices;
    int indices_size;
            
    VAOVIData(float* v, int v_s, unsigned int* i, int i_s) :
        vertices(v),
        vertices_size(v_s),
        indices(i),
        indices_size(i_s) {}
};
// World that contains all models in the scene
class Skybox {
    public:
        Skybox();
        // Constructor stores basic member variables
        Skybox(Shaders* shaders);
        // Deletes everything stored on Heap.
        ~Skybox();
        // Builds and loads all models in the scene.
        void loadModel();
        // Renders all models in world
        void draw();

    private:
        void addVAOToGraphicsCard(unsigned int* vaoHandle, int vaoNumber, 
                float* vertices, int vertices_size, 
                unsigned int* indices, int indices_size);
        VAOVIData generateData();
        void createTexture();
        unsigned int mProgramID;
        unsigned int mVAOHandle;        
        int mIndicesSize;
        Shaders *mShaders;
        unsigned int mTexID;
};

#endif


