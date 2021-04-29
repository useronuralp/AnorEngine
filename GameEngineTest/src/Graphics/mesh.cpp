#include "pch.h"
#include "mesh.h"
namespace GameEngineTest {
    namespace Graphics {
        Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<TextureInfo> textures)
            :modelMatrix(glm::mat4(1.0f)), viewMatrix(glm::mat4(1.0f)), projectionMatrix(glm::perspective(glm::radians(45.0f), 1920.0f / 1080.0f, 0.01f, 1000.0f))
        {
            this->vertices = vertices;
            this->indices = indices;
            this->textures = textures;
            setupMesh();
        }
        void Mesh::Draw(Shader& shader, glm::mat4 camera)
        {       
            unsigned int diffuseNr = 1;
            unsigned int specularNr = 1;
            for (unsigned int i = 0; i < textures.size(); i++)
            {
                glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
                // retrieve texture number (the N in diffuse_textureN)
                //std::string number;
                //std::string name = textures[i].type;
                //if (name == "texture_diffuse")
                //    number = std::to_string(diffuseNr++);
                //else if (name == "texture_specular")
                //    number = std::to_string(specularNr++);

                //shader.setUniform1i((name + number).c_str(), i);
                glBindTexture(GL_TEXTURE_2D, textures[i].id);
            }

            shader.enable();
            glBindVertexArray(VAO);
            int modelLocation = glGetUniformLocation(shader.getShaderID(), "ml_matrix");
            int viewLocation = glGetUniformLocation(shader.getShaderID(), "vw_matrix");
            int projectionLocation = glGetUniformLocation(shader.getShaderID(), "pr_matrix");
            glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
            glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(camera));
            glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

            //glDrawArrays(GL_TRIANGLES, 0, vertices.size()); // switch to this if you wanna draw with arrays.
            glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
            shader.disable();
        }

        void Mesh::setupMesh()
        {
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glGenBuffers(1, &EBO);

            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);

            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

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
