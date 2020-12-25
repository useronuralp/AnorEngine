#pragma once
#include <vector>
#include "shader.h"
#include "texture.h"
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

        class Mesh {
        public:
            // mesh data
            std::vector<Vertex>       vertices;
            //std::vector<unsigned int> indices;
            //std::vector<Texture>      textures;

            Mesh(std::vector<Vertex> vertices);
            void Draw(Shader& shader, glm::mat4 camera);
        private:

            glm::mat4 modelMatrix = glm::mat4(1.0f);
            glm::mat4 viewMatrix = glm::mat4(1.0f); //TODO: Fix this part later looks bad.
            glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), 1920.0f / 1080.0f, 0.1f, 100.0f);

            //  render data
            unsigned int VAO, VBO, EBO;

            void setupMesh();
        };

    }
}