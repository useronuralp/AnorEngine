#include "mesh.h"

namespace GameEngineTest {
    namespace Graphics {
        Mesh::Mesh(std::vector<Vertex> vertices)
        {
            this->vertices = vertices;
            //this->indices = indices;
            //this->textures = textures;

            setupMesh();
        }
        void Mesh::Draw(Shader& shader, glm::mat4 camera)
        {
            //for (unsigned int i = 1; i < textures.size(); i++)
            //{
            //    textures[i].Bind(i);
            //}
            shader.enable();
            glBindVertexArray(VAO);
            int modelLocation = glGetUniformLocation(shader.getShaderID(), "ml_matrix");
            int viewLocation = glGetUniformLocation(shader.getShaderID(), "vw_matrix");
            int projectionLocation = glGetUniformLocation(shader.getShaderID(), "pr_matrix");
            glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
            glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(camera));
            glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

            glDrawArrays(GL_TRIANGLES, 0, vertices.size());
            glBindVertexArray(0);
            shader.disable();
        }

        void Mesh::setupMesh()
        {
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            //glGenBuffers(1, &EBO);

            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);

            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

            //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            //glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

            // vertex positions
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));
            // vertex texture coords
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
            // normals
            glEnableVertexAttribArray(4);
            glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

            glBindVertexArray(0);
        }
    }
}
