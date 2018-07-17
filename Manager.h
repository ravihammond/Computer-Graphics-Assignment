#ifndef _MANAGER_H
#define _MANAGER_H

#include <vector>
#include <string>

// Handles the creation of Application, gets file input, and parses arguments.
// Starts the Object Viewer Application
class Manager {
    public:
        // Constructor instantiates required objects.
        Manager();
        // Deletes everything stored on Heap.
        ~Manager();
        // Reads arguments, error checks them, and stores them.
        void parseArgs(int argc, char** argv);
        void runApplication();

    private: 
        // File that map is stored on
        std::vector<std::string> mModelPaths;
};

#endif


