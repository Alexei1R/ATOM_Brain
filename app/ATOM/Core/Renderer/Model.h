//
// Created by toor on 11/10/23.
//

#ifndef ATOM_MODEL_H
#define ATOM_MODEL_H

#include "ATOM/atompch.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Mesh.h"
#include "stb_image.h"

namespace Atom
{



    class Model
    {
    public:
        // model data
        std::vector<Atom::Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
        std::vector<Atom::Mesh>    meshes;
        std::string directory;
        bool gammaCorrection;

        //Model(std::string const& path, bool gamma = false);
        Model() {};

        void Draw(Atom::Shader& shader);

    public:
        void loadModel(std::string const& path);


    private:
        // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
        void processNode(aiNode* node, const aiScene* scene);

        Atom::Mesh processMesh(aiMesh* mesh, const aiScene* scene);
        unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);
        // checks all material textures of a given type and loads the textures if they're not loaded yet.
        // the required info is returned as a Texture struct.
        std::vector<Atom::Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
    };



}


#endif //ATOM_MODEL_H
