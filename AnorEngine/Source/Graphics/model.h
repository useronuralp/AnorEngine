#pragma once
#include "mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
namespace AnorEngine {
    namespace Graphics {

        //Forward declarations
        class Texture;

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
            Ref<Texture>              m_DefaultDiffuse;
            Ref<Texture>              m_DefaultSpecular;
            Ref<Texture>              m_DefaultNormal;
            std::string               m_FileType = "None";
        private:
            void LoadModel(const std::filesystem::path& filePath);
            void ProcessNode(aiNode* node, const aiScene* scene);
            Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
            std::vector<Ref<Texture>> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName);
        public:
            void Draw(const Ref<Shader>& shader);
            const std::string& GetPath() { return m_RelativePath == "Not initialized" ? m_AbsolutePath : m_RelativePath; }
            const std::string& GetAbsolutePath() { return m_AbsolutePath; }
            inline void AddTexture(const Ref<Texture>& tex) { m_Textures.push_back(tex); }
        };
    }
}