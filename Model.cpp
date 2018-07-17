#include <stdio.h>
#include <iostream>
#include <vector>
#include <limits>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

// Latest version of tinyobjloader to load .obj files
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

// Used to load images
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Model.h"

// Values to be used when loading model to gpu
#define VALS_PER_VERT 3
#define VALS_PER_COLOR 3
#define VALS_PER_NORMAL 3
#define VALS_PER_TEXCOORD 2
#define TOTAL_VALS 11

using namespace std;

// Constructor instaliases member variables.
Model::Model() {}
Model::Model(float x, float y, float z, float yRot, Shaders *shaders) : 
    mX(x), 
    mY(y), 
    mZ(z),
    mYRot(yRot) {
    
    printf("Model constructor\n");
    mShaders = shaders;
}

// Deletes everything stored on Heap.
Model::~Model() {}

void Model::assertLoadModel(string filepath) {
    // Builds Model from path using tinyobjloader
    if (false == loadObjAndConvert(mBmin, mBmax, 
                &mDrawObjects, 
                mMaterials, 
                mTextures, 
                filepath)) {
        std::cerr << "Failed to load object." << std::endl;
        exit(1);
    }

}

// Gets the base directed of the filepath
static std::string GetBaseDir(const std::string& filepath) {
    if (filepath.find_last_of("/\\") != std::string::npos)
        return filepath.substr(0, filepath.find_last_of("/\\"));
    return "";
}

// Checks if file exists.
static bool FileExists(const std::string& abs_filename) {
    bool ret;
    FILE* fp = fopen(abs_filename.c_str(), "rb");
    if (fp) {
        ret = true;
        fclose(fp);
    } else {
        ret = false;
    }

    return ret;
}

// Loads object from .obj file, 
// saves all information into parameters, 
// loads all textures,
// Loads model onto gpu.
bool Model::loadObjAndConvert(float bmin[3], float bmax[3],
        vector<DrawObject>* drawObjects,
        vector<tinyobj::material_t>& materials,
        map<string, unsigned int>& textures,
        string filename) {
    // Object and attribute to be passed
    tinyobj::attrib_t attrib;
    vector<tinyobj::shape_t> shapes;

    // Extracts base directory from filename.
    std::string base_dir = GetBaseDir(filename);
    if (base_dir.empty()) {
        base_dir = ".";
    }

    // Adds the correct extension to base directory depending on OS.
#ifdef _WIN32
    base_dir += "\\";
#else
    base_dir += "/";
#endif

    // loads object from .obj file using tinyobjloader.
    string err;
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, 
            &err, filename.c_str(), base_dir.c_str());

    // Print any errors from loading the file.
    if (!err.empty()) {
        cerr << err << endl;
    }

    // Exit if .obj file failed to load.
    if (!ret) {
        std::cerr << "Failed to load " << filename << std::endl;
        return false;
    }

    // Append `default` material
    materials.push_back(tinyobj::material_t());

    // Load diffuse textures
    {
        for (size_t m = 0; m < materials.size(); m++) {
            tinyobj::material_t* mp = &materials[m];


            if (mp->diffuse_texname.length() > 0) {
                // Only load the texture if it is not already loaded
                if (textures.find(mp->diffuse_texname) == textures.end()) {
                    GLuint texture_id;
                    int w, h;
                    int comp;

                    // Gets texture filepath and ensures the file exists.
                    std::string texture_filename = mp->diffuse_texname;
                    if (!FileExists(texture_filename)) {
                        // Append base dir.
                        texture_filename = base_dir + mp->diffuse_texname;
                        if (!FileExists(texture_filename)) {
                            std::cerr << "Unable to find file: " << mp->diffuse_texname
                                << std::endl;
                            exit(1);
                        }
                    }

                    // Load texture image from file.
                    unsigned char* image =
                        stbi_load(texture_filename.c_str(), &w, &h, &comp, STBI_default);
                    if (!image) {
                        std::cerr << "Unable to load texture: " << texture_filename
                            << std::endl;
                        exit(1);
                    }

                    // Loads texture to graphics card
                    glActiveTexture(GL_TEXTURE0);
                    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
                    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
                    glGenTextures(1, &texture_id);
                    glBindTexture(GL_TEXTURE_2D, texture_id);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                    // Ensures texture is correct colour format.
                    if (comp == 3) {
                        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB,
                                GL_UNSIGNED_BYTE, image);
                    } else if (comp == 4) {
                        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
                                GL_UNSIGNED_BYTE, image);
                    } else {
                        assert(0);
                    }

                    stbi_image_free(image);
                    // Creates Mipmap filter.
                    glGenerateMipmap(GL_TEXTURE_2D); 
                    textures.insert(std::make_pair(mp->diffuse_texname, texture_id));
                }
            }
        }
    }

    // Initializes extent minimum and maximum's.
    bmin[0] = bmin[1] = bmin[2] = std::numeric_limits<float>::max();
    bmax[0] = bmax[1] = bmax[2] = -std::numeric_limits<float>::max();

    // Loops through all shapes in the .obj file.
    for (size_t s = 0; s < shapes.size(); s++) {
        // Object that hold the VAO and number of triangles for shape.
        DrawObject o;
        // pos(3float), normal(3float), color(3float)
        vector<float> vb; 

        // Loops through all faces in the shape.
        for (size_t f = 0; f < shapes[s].mesh.indices.size() / 3; f++) {
            // Extract vertex, normal, and texcoord for each face corner.
            tinyobj::index_t idx0 = shapes[s].mesh.indices[3 * f + 0];
            tinyobj::index_t idx1 = shapes[s].mesh.indices[3 * f + 1];
            tinyobj::index_t idx2 = shapes[s].mesh.indices[3 * f + 2];

            // Extract the id of the material that correlates to this face.
            int current_material_id = shapes[s].mesh.material_ids[f];

            // Check if id of material is invalid.
            if ((current_material_id < 0) ||
                    (current_material_id >= static_cast<int>(materials.size()))) {
                // Invaid material ID. Use default material.
                // Default material is added to the last item in `materials`.
                current_material_id = materials.size() - 1;  
            }

            // extract diffuse value from material.
            float diffuse[3];
            for (size_t i = 0; i < 3; i++) {
                diffuse[i] = materials[current_material_id].diffuse[i];
            }

            // Extract texture coordinates.
            float tc[3][2];
            // Check if any textcoord has been defined.
            if (attrib.texcoords.size() > 0) {
                // If any of the text coord coordinatesare invalid, set all to 0.
                if ((idx0.texcoord_index < 0) || (idx1.texcoord_index < 0) ||
                        (idx2.texcoord_index < 0)) {
                    tc[0][0] = 0.0f;
                    tc[0][1] = 0.0f;
                    tc[1][0] = 0.0f;
                    tc[1][1] = 0.0f;
                    tc[2][0] = 0.0f;
                    tc[2][1] = 0.0f;
                } else {
                    assert(attrib.texcoords.size() >
                            size_t(2 * idx0.texcoord_index + 1));
                    assert(attrib.texcoords.size() >
                            size_t(2 * idx1.texcoord_index + 1));
                    assert(attrib.texcoords.size() >
                            size_t(2 * idx2.texcoord_index + 1));

                    // Flip Y coord.
                    tc[0][0] = attrib.texcoords[2 * idx0.texcoord_index];
                    tc[0][1] = 1.0f - attrib.texcoords[2 * idx0.texcoord_index + 1];
                    tc[1][0] = attrib.texcoords[2 * idx1.texcoord_index];
                    tc[1][1] = 1.0f - attrib.texcoords[2 * idx1.texcoord_index + 1];
                    tc[2][0] = attrib.texcoords[2 * idx2.texcoord_index];
                    tc[2][1] = 1.0f - attrib.texcoords[2 * idx2.texcoord_index + 1];
                }
            } else {
                // If no texture coords exist, set to 0.
                tc[0][0] = 0.0f;
                tc[0][1] = 0.0f;
                tc[1][0] = 0.0f;
                tc[1][1] = 0.0f;
                tc[2][0] = 0.0f;
                tc[2][1] = 0.0f;
            }

            // Extract vertices from obj file.
            float v[3][3];
            for (int k = 0; k < 3; k++) {
                int f0 = idx0.vertex_index;
                int f1 = idx1.vertex_index;
                int f2 = idx2.vertex_index;
                assert(f0 >= 0);
                assert(f1 >= 0);
                assert(f2 >= 0);

                v[0][k] = attrib.vertices[3 * f0 + k];
                v[1][k] = attrib.vertices[3 * f1 + k];
                v[2][k] = attrib.vertices[3 * f2 + k];
                bmin[k] = std::min(v[0][k], bmin[k]);
                bmin[k] = std::min(v[1][k], bmin[k]);
                bmin[k] = std::min(v[2][k], bmin[k]);
                bmax[k] = std::max(v[0][k], bmax[k]);
                bmax[k] = std::max(v[1][k], bmax[k]);
                bmax[k] = std::max(v[2][k], bmax[k]);
            }

            // If obj file has normals, extract them.
            float n[3][3];
            {
                if (attrib.normals.size() > 0) {
                    int f0 = idx0.normal_index;
                    int f1 = idx1.normal_index;
                    int f2 = idx2.normal_index;
                    assert(f0 >= 0);
                    assert(f1 >= 0);
                    assert(f2 >= 0);
                    for (int k = 0; k < 3; k++) {
                        n[0][k] = attrib.normals[3 * f0 + k];
                        n[1][k] = attrib.normals[3 * f1 + k];
                        n[2][k] = attrib.normals[3 * f2 + k];
                    }
                } else {
                    // If no normals are set, set them to 0.
                    for (int i = 0; i < 3; i++) {
                        n[i][0] = 0;
                        n[i][1] = 0;
                        n[i][2] = 0;
                    }
                }
            }

            for (int k = 0; k < 3; k++) {
                vb.push_back(v[k][0]);
                vb.push_back(v[k][1]);
                vb.push_back(v[k][2]);
                vb.push_back(n[k][0]);
                vb.push_back(n[k][1]);
                vb.push_back(n[k][2]);

                // Use normal as color.
                float c[3] = {n[k][0], n[k][1], n[k][2]};
                float len2 = c[0] * c[0] + c[1] * c[1] + c[2] * c[2];
                if (len2 > 0.0f) {
                    float len = sqrtf(len2);

                    c[0] /= len;
                    c[1] /= len;
                    c[2] /= len;
                }
                // Add colors to color vector.
                vb.push_back(c[0] * 0.5 + 0.5);
                vb.push_back(c[1] * 0.5 + 0.5);
                vb.push_back(c[2] * 0.5 + 0.5);

                vb.push_back(tc[k][0]);
                vb.push_back(tc[k][1]);
            }
        }

        // Set the vao for drawobject.
        o.vao = 0;
        o.numTriangles = 0;

        if (shapes[s].mesh.material_ids.size() > 0 &&
                shapes[s].mesh.material_ids.size() > s) {
            o.material_id = shapes[s].mesh.material_ids[0]; 
        } else {
            o.material_id = materials.size() - 1; 
        }

        // Add shape data to GPU.
        if (vb.size() > 0) {
            glGenVertexArrays(1, &o.vao);
            glBindVertexArray(o.vao);


            unsigned int buffer[1];
            glGenBuffers(1, buffer);

            // Set vertex position
            glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
            glBufferData(GL_ARRAY_BUFFER, vb.size() * sizeof(float), &vb.at(0),
                    GL_STATIC_DRAW);
            // Vertex data.
            glVertexAttribPointer(0, VALS_PER_VERT, GL_FLOAT, GL_FALSE, 
                    11 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

            // Normal data.
            glVertexAttribPointer(1, VALS_PER_NORMAL, GL_FLOAT, GL_FALSE, 
                    11 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);

            // Color data.
            glVertexAttribPointer(2, VALS_PER_COLOR, GL_FLOAT, GL_FALSE, 
                    11 * sizeof(float), (void*)(6 * sizeof(float)));
            glEnableVertexAttribArray(2);

            // Textcood data.
            glVertexAttribPointer(3, VALS_PER_TEXCOORD, GL_FLOAT, GL_FALSE, 
                    11 * sizeof(float), (void*)(9 * sizeof(float)));
            glEnableVertexAttribArray(3);


            o.numTriangles = vb.size() / 11 / 3;
        }

        // Append final object to vector.
        drawObjects->push_back(o);
    }

    return true;  
}
