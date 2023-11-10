//
// Created by toor on 11/10/23.
//

#ifndef ATOM_MESH_H
#define ATOM_MESH_H
#include "ATOM/atompch.h"
#include "Shader.h"

#define MAX_BONE_INFLUENCE 4

namespace Atom
{

    struct Vertex
    {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoords;
        glm::vec3 Tangent;
        glm::vec3 Bitangent;
        int m_BoneIDs[MAX_BONE_INFLUENCE];
        float m_Weights[MAX_BONE_INFLUENCE];
    };
    struct Texture
    {
        unsigned int id;
        std::string type;
        std::string path;
    };



    class Mesh
    {
    private:
        std::vector<Vertex>       vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture>      textures;
        unsigned int VAO , VBO, EBO;

    public:
        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

        void Draw(Atom::Shader& shader);

    private:
        void setupMesh();
    };



}





#endif //ATOM_MESH_H
