#ifndef _GROUND_H
#define _GROUND_H

#include "Shaders.h"

// Struct to deal with passing the generates arrays back as one return statement to be
// added the to graphics card for open gel to use in the pipeline
struct VAOData { 
    float* vertices;
    int vertices_size;
    unsigned int* indices;
    int indices_size;
    float* normals;
    int normals_size;
    float* tex_coords;
    int tex_coords_size;
            
    VAOData(float* v, int v_s, unsigned int* i, int i_s, 
            float* n, int n_s, float* t, int t_s) : 
        vertices(v),
        vertices_size(v_s),
        indices(i),
        indices_size(i_s),
        normals(n),
        normals_size(n_s),
        tex_coords(t),
        tex_coords_size(t_s){}
};

// World that contains all models in the scene
class Ground {
    public:
        Ground();
        // Constructor stores basic member variables
        Ground(Shaders* shaders);

        // Deletes everything stored on Heap.
        ~Ground();

        // Builds and loads all models in the scene.
        void loadModel();

        // Renders all models in world
        void draw();

        // Returns program ID
    private:
        void addVAOToGraphicsCard(unsigned int* vaoHandle, int vaoNumber, 
                float* vertices, int vertices_size, 
                unsigned int* indices, int indices_size, 
                float* normals, int normals_size,
                float* tex_coords, int tex_coords_size);
        VAOData generateData();
        void createTexture();
        unsigned int mProgramID;
        unsigned int mVAOHandle;        
        int mIndicesSize;
        Shaders *mShaders;
        unsigned int mTexID;
};

#endif

