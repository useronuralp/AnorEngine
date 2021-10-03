#pragma once
#include <glm.hpp>
namespace AnorEngine {
    namespace Graphics {

        //Forward declerations
        class Shader;
        class Texture;
        class VertexArray;
        class Vertex;

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