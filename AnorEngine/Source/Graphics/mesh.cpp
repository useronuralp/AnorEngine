#include "pch.h"
#include "mesh.h"
namespace AnorEngine {
    namespace Graphics {
        Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Ref<Texture>>& textures)
            :m_ModelMatrix(glm::mat4(1.0f))
        {
            this->m_Vertices = vertices;
            this->m_Indices = indices;
            this->m_Textures = textures;
            SetupMesh();
        }
        void Mesh::Draw(const Ref<Shader>& shader)
        {       
            shader->Enable();
            for (unsigned int i = 0; i < m_Textures.size(); i++)
            {
                glActiveTexture(GL_TEXTURE0 + m_Textures[i]->GetTextureID()); // activate proper texture unit before binding
                glBindTexture(GL_TEXTURE_2D, m_Textures[i]->GetTextureID());
            }
            for (auto& texture : m_Textures)
            {
                //TODO: Handle the case where not all the maps are present in the model file. Example: Model is missing the specular map but has the diffuse. It should still reneder diffuse.
                if (texture->GetType() == "texture_diffuse")
                    shader->UploadUniform("u_TextureSamplerDiffuse", sizeof(texture->GetTextureID()), &texture->GetTextureID());
                else if (texture->GetType() == "texture_specular")
                    shader->UploadUniform("u_TextureSamplerSpecular", sizeof(texture->GetTextureID()), &texture->GetTextureID());
            }
            m_VAO->Bind();

            //glDrawArrays(GL_TRIANGLES, 0, m_Vertices.size()); // switch to this if you wanna draw with arrays.
            glDrawElements(GL_TRIANGLES, (GLsizei)m_Indices.size(), GL_UNSIGNED_INT, 0);
            m_VAO->Unbind();
            shader->Disable();
        }
        void Mesh::SetupMesh()
        {
            BufferLayout BufferLayout = {
                {ShaderDataType::vec3, "a_Position", 0},
                {ShaderDataType::vec3, "a_Normal", 1},
                {ShaderDataType::vec2, "a_UV", 2},
            };

            std::vector<float> arr;
            for (int i = 0; i < m_Vertices.size(); i++)
            {
                arr.push_back(m_Vertices[i].Position.x);
                arr.push_back(m_Vertices[i].Position.y);
                arr.push_back(m_Vertices[i].Position.z);

                arr.push_back(m_Vertices[i].Normal.x);
                arr.push_back(m_Vertices[i].Normal.y);
                arr.push_back(m_Vertices[i].Normal.z);

                arr.push_back(m_Vertices[i].TexCoords.x);
                arr.push_back(m_Vertices[i].TexCoords.y);

            }
            m_VAO = std::make_shared<VertexArray>();
            m_VAO->AddVertexBuffer(std::make_shared<VertexBuffer>(&arr[0], arr.size() * sizeof(float), BufferLayout));
            m_VAO->SetIndexBuffer(std::make_shared<IndexBuffer>(&m_Indices[0], m_Indices.size()));
        }    
    }
}
