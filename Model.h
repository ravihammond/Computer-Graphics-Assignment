#ifndef _MODEL_H
#define _MODEL_H

#include <vector>
#include <map>

#include "Shaders.h"

//#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

// Creates struct that holds information about each shape in model.
typedef struct {
    unsigned int vao;
    int numTriangles;
	size_t material_id;
} DrawObject;

// Loads Models using tinyobjlaoder
class Model {
    public:
        // Constructor stores basic member variables
        Model();
        Model(float x, float y, float z, float yRot, Shaders* shaders);
        // Deletes everything stored on Heap.
        ~Model();
        // Builds and loads all models in the scene.
        virtual void loadModel() = 0;
        // Renders all models in world
        virtual void draw() = 0;

    protected:
        void assertLoadModel(std::string filepath);

        // Loads object from .obj file, 
        bool loadObjAndConvert(float bmin[3], float bmax[3],
                std::vector<DrawObject>* drawObjects,
                std::vector<tinyobj::material_t>& materials,
                std::map<std::string, unsigned int>& textures,
                std::string filename);
        Shaders* mShaders;
        unsigned int mProgramID;

        // Coordinates of model in world coordinates.
        float mX;
        float mY;
        float mZ;
        // Y axis rotation of model 
        float mYRot;

        // Vector of all shapes held within Model.
        std::vector<DrawObject> mDrawObjects;
        // Vector of all materials this Model uses.
        std::vector<tinyobj::material_t> mMaterials;
        // Mappin of texture file names -> texture id.
        std::map<std::string, unsigned int> mTextures;

        // Largest vertex coordinate for each axis individually.
        float mBmin[3];
        // Smallest vertex coordinate for each axis individually.
        float mBmax[3];
};

#endif

