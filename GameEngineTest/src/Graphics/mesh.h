#pragma once
#include <vector>
#include "shader.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

namespace GameEngineTest {
    namespace Graphics {

        struct Vertex {
            glm::vec3 Position;
            glm::vec2 TexCoords;
            glm::vec3 Normal;
        };

        struct TextureInfo {
            unsigned int id;
            std::string type;
            std::string path;
        };

        class Mesh {
        public:
            std::vector<Vertex>       vertices;
            std::vector<unsigned int> indices;
            std::vector<TextureInfo>  textures;
        private:
            glm::mat4 modelMatrix;
            glm::mat4 viewMatrix;
            glm::mat4 projectionMatrix;
            //  render data
            unsigned int VAO, VBO, EBO;
        private:
            void setupMesh();
        public:
            // mesh data
            Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<TextureInfo> textures);
            void Draw(Shader& shader, glm::mat4 camera);
            glm::mat4& getModelMatrix() { return modelMatrix; }
        };

    }
}