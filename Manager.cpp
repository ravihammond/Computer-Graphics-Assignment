#include <stdio.h>

#include "Manager.h"
#include "Application.h"

using namespace std;

// Constructor instaliases member variables.
Manager::Manager() {}

// Reads arguments, error checks them, and stores them.
void Manager::parseArgs(int argc, char** argv) {
    // Check to make sure there are enough arguments
    if (argc < 2 || argc > 3) {
        printf("Argument Error: Please make sure there is 1 or 2 arguments\n");
        exit(1);
    }

    // Extract object one file path from arguments.
    mModelPaths.push_back(argv[1]);

    if (argc == 3) {
        // Extract object two file path from arguments.
        mModelPaths.push_back(argv[2]);
    }
}


// Create Maze Application
void Manager::runApplication() {
    Application app;
    app.init(mModelPaths);
}

// Deletes everything stored on Heap.
Manager::~Manager() {

}
