#pragma once
#include "shader.h"
#include <vector>
#include "texture.h"
#include <string.h>
#include "mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


namespace GameEngineTest {
    namespace Graphics {
        class Model 
        {
        public:
            Model(const char* path)
                :vertexCount(0)
            {
                loadModel(path);
            }
            void Draw(Shader& shader, glm::mat4 camera);
        private:
            // model data
            std::vector<Mesh> meshes;
            std::string directory;
            int vertexCount;
            void loadModel(std::string path);
            void processNode(aiNode* node, const aiScene* scene);
            Mesh processMesh(aiMesh* mesh, const aiScene* scene);
            std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
        public:
            std::vector<Mesh> getMeshes() { return meshes; }
            int getVertexCount() { return vertexCount; }
        };
    }
}