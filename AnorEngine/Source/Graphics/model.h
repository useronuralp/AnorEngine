#pragma once
#include "mesh.h"
namespace AnorEngine {
    namespace Graphics {
        class ANOR_API Model
        {
        public:
            Model(const std::filesystem::path& filePath);
        private:
            std::string               m_RelativePath = "Not initialized";
            std::string               m_AbsolutePath;
            std::vector<Mesh>         m_Meshes;
            std::string               m_Directory;
            std::vector<Ref<Texture>> m_Textures;
        private:
            void LoadModel(const std::filesystem::path& filePath);
            void ProcessNode(aiNode* node, const aiScene* scene);
            Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
            std::vector<Ref<Texture>> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
        public:
            void Draw(const Ref<Shader>& shader);
            const std::string& GetPath() { return m_RelativePath == "Not initialized" ? m_AbsolutePath : m_RelativePath; }
            const std::string& GetAbsolutePath() { return m_AbsolutePath; }
        };
    }
}