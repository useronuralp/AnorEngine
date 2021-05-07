#pragma once
#include "shader.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "Camera.h"
namespace AnorEngine {
    namespace Graphics {

        struct ANOR_API Vertex {
            glm::vec3 Position;
            glm::vec2 TexCoords;
            glm::vec3 Normal;
        };

        struct ANOR_API TextureInfo {
            unsigned int id;
            std::string type;
            std::string path;
        };

        class ANOR_API Mesh {
        public:
            std::vector<Vertex>       vertices;
            std::vector<unsigned int> indices;
            std::vector<TextureInfo>  textures;
        private:
            glm::mat4 modelMatrix;
            //  render data
            unsigned int VAO, VBO, EBO;
        private:
            void setupMesh();
        public:
            // mesh data
            Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<TextureInfo> textures);
            void Draw(Shader& shader, PerspectiveCamera* camera);
            inline glm::mat4& getModelMatrix() { return modelMatrix; }
        };

    }
}