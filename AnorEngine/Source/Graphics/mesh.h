#pragma once
#include "shader.h"
#include <glm.hpp>
#include <../Vendor/stb_image/stb_image.h>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <Graphics/Buffers/VertexArray.h>
#include <Graphics/texture.h>
#include "EditorCamera.h"

namespace AnorEngine {
    namespace Graphics {
        struct ANOR_API Vertex {
            glm::vec3 Position;
            glm::vec2 TexCoords;
            glm::vec3 Normal;
            glm::vec3 Tangent;
            glm::vec3 Bitangent;
        };

        class ANOR_API Mesh {
        private:
            //The vectors are temporary containers before we upload the data into GPU.
            std::vector<Vertex>       m_Vertices;
            std::vector<unsigned int> m_Indices;
            std::vector<Ref<Texture>> m_Textures;
            glm::mat4                 m_ModelMatrix;
        private:
            Ref<VertexArray>          m_VAO;
        private:
            void SetupMesh();
        public:
            Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Ref<Texture>>& textures);
        public:
            void Draw(const Ref<Shader>& shader);
        };    
    }
}