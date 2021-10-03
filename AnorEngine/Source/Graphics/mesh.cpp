#include "pch.h"
#include "mesh.h"
#include "shader.h"
#include "EditorCamera.h"
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <Graphics/texture.h>
#include <Graphics/Buffers/VertexArray.h>
#include <stb_image.h>
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
                glActiveTexture(GL_TEXTURE0 + m_Textures[i]->GetTextureID() % 32); // activate proper texture unit before binding
                glBindTexture(GL_TEXTURE_2D, m_Textures[i]->GetTextureID());
            }
            for (auto& texture : m_Textures)
            {
                if (texture->GetType() == "texture_diffuse")
                    shader->UploadUniform("u_DiffuseMap", sizeof(texture->GetTextureID()), &texture->GetTextureID());
                else if (texture->GetType() == "texture_specular")
                    shader->UploadUniform("u_SpecularMap", sizeof(texture->GetTextureID()), &texture->GetTextureID());
                else if (texture->GetType() == "texture_normalMap")
                    shader->UploadUniform("u_NormalMap", sizeof(texture->GetTextureID()), &texture->GetTextureID());
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
                {ShaderDataType::vec3, "a_Tangnent", 3},
                {ShaderDataType::vec3, "a_Bitangent", 4}
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
                //Since stbi doesnt let me flip the texture vertically upon loading, I do it here manually. (Texture needs to be flipped because OpenGL accepts flipped UVs on y-axis, weird I know.)
                arr.push_back(1.0f - m_Vertices[i].TexCoords.y);

                arr.push_back(m_Vertices[i].Tangent.x);
                arr.push_back(m_Vertices[i].Tangent.y);
                arr.push_back(m_Vertices[i].Tangent.z);

                arr.push_back(m_Vertices[i].Bitangent.x);
                arr.push_back(m_Vertices[i].Bitangent.y);
                arr.push_back(m_Vertices[i].Bitangent.z);
            }
            m_VAO = std::make_shared<VertexArray>();
            m_VAO->AddVertexBuffer(std::make_shared<VertexBuffer>(&arr[0], arr.size() * sizeof(float), BufferLayout));
            m_VAO->SetIndexBuffer(std::make_shared<IndexBuffer>(&m_Indices[0], m_Indices.size()));
        }    
    }
}
