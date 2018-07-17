#ifndef _TREE_H
#define _TREE_H

#include "Model.h"

class Tree: public Model {
    public:
        Tree(float x, float y, float z, float yRot, Shaders* shaders);
        // Builds and loads all models in the scene.
        void loadModel();
        // Renders all models in world
        void draw();
};

#endif


