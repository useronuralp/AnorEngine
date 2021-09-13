#include "pch.h"
#include "model.h"
namespace AnorEngine {
	namespace Graphics {
		Model::Model(const std::filesystem::path& filePath)
		{
			m_AbsolutePath = filePath.string();
			std::string directory = filePath.string().substr(0, filePath.string().find_last_of("\\/"));
			std::string folderName = directory.substr(directory.find_last_of("\\/") + 1, directory.length());
			std::string objName = filePath.string().substr(filePath.string().find_last_of("\\/") + 1, filePath.string().length());
			m_RelativePath = "Models\\" + folderName + "\\" + objName;

			LoadModel(filePath);
		}
		void Model::Draw(const Ref<Shader>& shader)
		{
			for (unsigned int i = 0; i < m_Meshes.size(); i++)
				m_Meshes[i].Draw(shader);
		}
		void Model::LoadModel(const std::filesystem::path& filePath)
		{
			m_AbsolutePath = filePath.string();
			Assimp::Importer import;
			const aiScene* scene = import.ReadFile(filePath.string(), aiProcess_Triangulate | aiProcess_FlipUVs);

			if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
			{	
				CRITICAL_ASSERT("ERROR::ASSIMP:: {0}", import.GetErrorString());
				return;
			}
			m_Directory = filePath.string().substr(0, filePath.string().find_last_of("\\/"));
			INFO("Directory name: {0}", m_Directory);
			ProcessNode(scene->mRootNode, scene);
		}
		void Model::ProcessNode(aiNode* node, const aiScene* scene)
		{
			// process all the node's meshes (if any)
			for (unsigned int i = 0; i < node->mNumMeshes; i++)
			{
				aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
				m_Meshes.push_back(ProcessMesh(mesh, scene));
			}
			// then do the same for each of its children
			for (unsigned int i = 0; i < node->mNumChildren; i++)
			{
				ProcessNode(node->mChildren[i], scene);
			}
		}
		Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
		{
			std::vector<Vertex> vertices;
			std::vector<unsigned int> indices;
			std::vector<Ref<Texture>> textures;

			for (unsigned int i = 0; i < mesh->mNumVertices; i++)
			{
				Vertex vertex;
				// process vertex positions, normals and texture coordinates
				glm::vec3 vector;
				vector.x = mesh->mVertices[i].x;
				vector.y = mesh->mVertices[i].y;
				vector.z = mesh->mVertices[i].z;
				vertex.Position = vector;

				vector.x = mesh->mNormals[i].x;
				vector.y = mesh->mNormals[i].y;
				vector.z = mesh->mNormals[i].z;
				vertex.Normal = vector;

				if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
				{
					glm::vec2 vector;
					vector.x = mesh->mTextureCoords[0][i].x;
					vector.y = mesh->mTextureCoords[0][i].y;
					vertex.TexCoords = vector;
				}
				else
					vertex.TexCoords = glm::vec2(0.0f, 0.0f);	
				vertices.push_back(vertex);
			}
			// process indices
			for (unsigned int i = 0; i < mesh->mNumFaces; i++)
			{
				aiFace face = mesh->mFaces[i];
				for (unsigned int j = 0; j < face.mNumIndices; j++)
					indices.push_back(face.mIndices[j]);
			}
			// process material
			if (mesh->mMaterialIndex >= 0)
			{
				aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
				std::vector<Ref<Texture>> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
				textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
				std::vector<Ref<Texture>> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
				textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
			}
			return Mesh(vertices, indices, textures);
		}
		std::vector<Ref<Texture>> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
		{
			std::vector<Ref<Texture>> textures;
			std::string folderName = m_Directory.substr(m_Directory.find_last_of("\\/") + 1, m_Directory.length());
			for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
			{
				aiString str;
				mat->GetTexture(type, i, &str);
				std::string relativePath = "Models\\" + folderName + "\\" + str.C_Str();			
				bool skip = false;
				for (unsigned int j = 0; j < m_Textures.size(); j++)
				{
					
					if (std::strcmp(m_Textures[j]->GetPath().c_str(), relativePath.c_str()) == 0)
					{
						textures.push_back(m_Textures[j]);
						skip = true;
						break;
					}
				}
				if (!skip)
				{   // if texture hasn't been loaded already, load it
					
					Ref<Texture> texture = std::make_shared<Texture>("Models\\" + folderName + "\\" + str.C_Str());
					texture->SetType(typeName);
					textures.push_back(texture);
					m_Textures.push_back(texture); // add to loaded textures
				}
			}
			return textures;
		}
	}
}
