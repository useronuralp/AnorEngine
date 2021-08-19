#include "pch.h"
#include "mesh.h"
namespace AnorEngine {
    namespace Graphics {
        Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<TextureInfo>& textures)
            :modelMatrix(glm::mat4(1.0f))
        {
            this->vertices = vertices;
            this->indices = indices;
            this->textures = textures;
            setupMesh();
        }
        void Mesh::Draw(const Ref<Shader> shader, const Ref<PerspectiveCamera> camera)
        {       
            unsigned int diffuseNr = 1;
            unsigned int specularNr = 1;
            shader->Enable();
            for (unsigned int i = 0; i < textures.size(); i++)
            {
                glActiveTexture(GL_TEXTURE0 + textures[i].id); // activate proper texture unit before binding
                glBindTexture(GL_TEXTURE_2D, textures[i].id);
                //shader->UploadInteger("textureSampler", textures[i].id);
            }
            //shader->UploadInteger("textureSampler", textures[0].id);
            glBindVertexArray(VAO);
            int modelLocation = glGetUniformLocation(shader->getShaderID(), "ml_matrix");
            int viewLocation = glGetUniformLocation(shader->getShaderID(), "vw_matrix");
            int projectionLocation = glGetUniformLocation(shader->getShaderID(), "pr_matrix");
            glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
            glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(glm::lookAt(camera->cameraPos, camera->cameraPos + camera->cameraFront, camera->cameraUp)));
            glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(camera->m_ProjectionMatrix));

            //glDrawArrays(GL_TRIANGLES, 0, vertices.size()); // switch to this if you wanna draw with arrays.
            glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
            shader->Disable();
        }

        void Mesh::setupMesh()
        {
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glGenBuffers(1, &EBO);

            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
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
            glBindBuffer(VBO, 0);
            glBindBuffer(EBO, 0);
        }
    }
}
