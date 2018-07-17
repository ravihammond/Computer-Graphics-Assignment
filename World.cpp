#define GLFW_INCLUDE_NONE

#include <stdio.h>

#include "World.h"
#include "Tree.h"
#include "Car.h"

using namespace std;

// Constructor stores basic member variables
World::World() {}
World::World(Shaders* shaders, Player* player) {
    this->mShaders = shaders;
    mGround = Ground(mShaders);
    mSkybox = Skybox(mShaders);

    mCar = new Car(0.0f, 0.88f, 0.0f, 0.0f, mShaders, player);

    vector<pair<float,float>> treeLocations = genTreeLocations();
    player->setTreeCoords(treeLocations);

    for (auto coord :  treeLocations) {
        mModels.push_back(new Tree(coord.first, 0.0f, coord.second, 0.0f, mShaders));
    }
}

// Deletes everything stored on Heap.
World::~World() {}

// Initialised view vectors
void World::buildModels() {
    mGround.loadModel();
    mSkybox.loadModel();
    mCar->loadModel();

    for (Model* model : mModels) {
        model->loadModel();
    }
}

// Renders all models in world
void World::render() {
    mSkybox.draw();
    mGround.draw();
    mCar->draw();

    for (Model* model : mModels) {
        model->draw();
    }
}

vector<pair<float,float>>  World::genTreeLocations() {
    vector<pair<float,float>> treeLocations;
    treeLocations.push_back(make_pair(-22.388276f, -22.971703f));
    treeLocations.push_back(make_pair(-48.181614f, 38.699745f));
    treeLocations.push_back(make_pair(-3.947164f, -9.529412f));
    treeLocations.push_back(make_pair(-13.445799f, 38.308818f));
    treeLocations.push_back(make_pair(34.389728f, -43.328973f));
    treeLocations.push_back(make_pair(-20.081320f, -48.038461f));
    treeLocations.push_back(make_pair(-3.383376f, -42.798697f));
    treeLocations.push_back(make_pair(2.902425f, 2.275782f));
    treeLocations.push_back(make_pair(37.402351f, 34.652002f));
    treeLocations.push_back(make_pair(-31.448599f, -42.532577f));
    treeLocations.push_back(make_pair(-14.364833f, -2.419769f));
    treeLocations.push_back(make_pair(-29.737693f, 5.585268f));
    treeLocations.push_back(make_pair(-44.944284f, 35.711121f));
    treeLocations.push_back(make_pair(8.844098f, 29.031490f));
    treeLocations.push_back(make_pair(26.802553f, -33.307492f));
    treeLocations.push_back(make_pair(37.069931f, -24.060268f));
    treeLocations.push_back(make_pair(2.650122f, 32.559951f));
    treeLocations.push_back(make_pair(-4.859919f, 26.506904f));
    treeLocations.push_back(make_pair(-0.922136f, 37.124611f));
    treeLocations.push_back(make_pair(-26.929062f, 34.332797f));
    treeLocations.push_back(make_pair(41.336613f, 12.961791f));
    treeLocations.push_back(make_pair(-8.286955f, 27.017215f));
    treeLocations.push_back(make_pair(43.871206f, -4.412150f));
    treeLocations.push_back(make_pair(42.221938f, -20.802264f));
    treeLocations.push_back(make_pair(-5.878662f, -27.060844f));
    treeLocations.push_back(make_pair(-12.004931f, 4.103399f));
    treeLocations.push_back(make_pair(5.629494f, 1.309611f));
    treeLocations.push_back(make_pair(45.476266f, -22.308970f));
    treeLocations.push_back(make_pair(44.686650f, 9.830363f));
    treeLocations.push_back(make_pair(11.425902f, -47.066365f));
    treeLocations.push_back(make_pair(-29.063938f, -29.747115f));
    treeLocations.push_back(make_pair(-46.635703f, -22.821410f));
    treeLocations.push_back(make_pair(-27.308074f, -20.348851f));
    treeLocations.push_back(make_pair(-4.419321f, 3.361021f));
    treeLocations.push_back(make_pair(38.917983f, 43.231520f));
    treeLocations.push_back(make_pair(-41.740213f, -20.159311f));
    treeLocations.push_back(make_pair(43.399209f, -12.576751f));
    treeLocations.push_back(make_pair(-37.217304f, 41.601592f));
    treeLocations.push_back(make_pair(-8.562784f, 36.220563f));
    treeLocations.push_back(make_pair(27.077268f, -39.730405f));
    treeLocations.push_back(make_pair(-6.358754f, -1.903765f));
    treeLocations.push_back(make_pair(-0.964279f, 37.605356f));
    treeLocations.push_back(make_pair(3.302472f, -16.917935f));
    treeLocations.push_back(make_pair(-42.653651f, 3.607607f));
    treeLocations.push_back(make_pair(40.474809f, 19.004743f));
    treeLocations.push_back(make_pair(-41.921844f, 27.498857f));
    treeLocations.push_back(make_pair(29.971156f, -6.093509f));
    treeLocations.push_back(make_pair(-45.709227f, -37.558148f));
    treeLocations.push_back(make_pair(31.723159f, 12.775166f));
    treeLocations.push_back(make_pair(40.621543f, 45.242119f));
    treeLocations.push_back(make_pair(48.507870f, -17.072968f));
    treeLocations.push_back(make_pair(23.598211f, -2.632532f));
    treeLocations.push_back(make_pair(37.006268f, 27.067403f));
    treeLocations.push_back(make_pair(-48.981453f, -28.139972f));
    treeLocations.push_back(make_pair(-0.795057f, -43.570308f));
    treeLocations.push_back(make_pair(34.479454f, 48.686226f));
    treeLocations.push_back(make_pair(-10.993982f, 47.060298f));
    treeLocations.push_back(make_pair(-27.658821f, 2.460095f));
    treeLocations.push_back(make_pair(-0.563139f, 47.773239f));
    treeLocations.push_back(make_pair(-30.184181f, 24.301161f));
    treeLocations.push_back(make_pair(-10.897055f, -37.033817f));
    treeLocations.push_back(make_pair(-12.561382f, 11.440918f));
    treeLocations.push_back(make_pair(39.831048f, 46.869246f));
    treeLocations.push_back(make_pair(-5.571751f, 26.549146f));
    treeLocations.push_back(make_pair(-5.431953f, 30.590698f));
    treeLocations.push_back(make_pair(46.537678f, 32.562635f));
    treeLocations.push_back(make_pair(45.750542f, 8.284370f));
    treeLocations.push_back(make_pair(-15.134271f, 46.508246f));
    treeLocations.push_back(make_pair(-9.070860f, -35.219864f));
    treeLocations.push_back(make_pair(-11.868318f, 8.880466f));
    treeLocations.push_back(make_pair(20.693273f, 3.361742f));
    treeLocations.push_back(make_pair(8.005878f, 1.686564f));
    treeLocations.push_back(make_pair(27.009696f, -20.319053f));
    treeLocations.push_back(make_pair(35.655176f, 14.585892f));
    treeLocations.push_back(make_pair(-22.640965f, -22.147812f));
    treeLocations.push_back(make_pair(-1.454308f, -16.143101f));
    treeLocations.push_back(make_pair(16.577747f, 15.258609f));
    treeLocations.push_back(make_pair(-31.259565f, -3.632611f));
    treeLocations.push_back(make_pair(-4.341460f, -48.468944f));
    treeLocations.push_back(make_pair(40.480510f, -45.733741f));
    treeLocations.push_back(make_pair(21.615572f, -43.549893f));
    treeLocations.push_back(make_pair(-2.691245f, 14.360432f));
    treeLocations.push_back(make_pair(-26.123094f, -22.280835f));
    treeLocations.push_back(make_pair(-43.146339f, 34.606131f));
    treeLocations.push_back(make_pair(-13.642328f, 22.295264f));
    treeLocations.push_back(make_pair(19.682765f, 16.663973f));
    treeLocations.push_back(make_pair(42.351999f, -28.508811f));
    treeLocations.push_back(make_pair(-3.854633f, -7.349119f));
    treeLocations.push_back(make_pair(-19.279626f, -17.269996f));
    treeLocations.push_back(make_pair(19.300292f, -15.028863f));
    treeLocations.push_back(make_pair(-16.603020f, 31.711044f));
    treeLocations.push_back(make_pair(-34.916900f, 38.191507f));
    treeLocations.push_back(make_pair(-0.695305f, 20.626922f));
    treeLocations.push_back(make_pair(-3.046537f, 45.470901f));
    treeLocations.push_back(make_pair(-30.267992f, 28.769383f));
    treeLocations.push_back(make_pair(44.478556f, -4.948462f));
    treeLocations.push_back(make_pair(6.702279f, 48.909740f));
    treeLocations.push_back(make_pair(-5.712020f, -16.079905f));
    treeLocations.push_back(make_pair(14.923996f, -22.244889f));
    
    return treeLocations;
}

