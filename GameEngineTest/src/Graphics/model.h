#pragma once
#include "../vendor/stb_image/stb_image.h"
#include "mesh.h"
#include <string.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace GameEngineTest {
    namespace Graphics {
        class Model 
        {
        public:
            Model(const char* path)
            {
                loadModel(path);
            }
        private:
            // model data
            std::vector<Mesh>        meshes;
            std::string              directory;
            std::vector<TextureInfo> textures_loaded;
        private:
            void loadModel(std::string path);
            void processNode(aiNode* node, const aiScene* scene);
            Mesh processMesh(aiMesh* mesh, const aiScene* scene);
            std::vector<TextureInfo> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
            unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);
        public:
            std::vector<Mesh> getMeshes() { return meshes; }
            void Draw(Shader& shader, glm::mat4 camera);
            void rotate(const float& degree, const float& x, const float& y, const float& z);
            void translate(const float& x, const float& y, const float& z);
            void scale(const float& x, const float& y, const float& z);
        };
    }
}