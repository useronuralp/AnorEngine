#include "pch.h"
#include "model.h"
#include "Graphics/texture.h"
namespace AnorEngine {
	namespace Graphics {
		Model::Model(const std::filesystem::path& filePath)
		{
			m_AbsolutePath = filePath.string();
			m_FileType = m_AbsolutePath.substr(m_AbsolutePath.find_last_of('.') + 1, m_AbsolutePath.length());
			std::string directory = m_AbsolutePath.substr(0, m_AbsolutePath.find_last_of("\\/"));
			std::string folderName = directory.substr(directory.find_last_of("\\/") + 1, directory.length());
			std::string objName = m_AbsolutePath.substr(m_AbsolutePath.find_last_of("\\/") + 1, m_AbsolutePath.length());
			m_RelativePath = "Models\\" + folderName + "\\" + objName;

			m_DefaultDiffuse = std::make_shared<Texture>("Textures\\WhiteTexture.png");
			m_DefaultDiffuse->SetType("texture_diffuse");
			m_DefaultSpecular = std::make_shared<Texture>("Textures\\WhiteTexture.png");
			m_DefaultSpecular->SetType("texture_specular");
			m_DefaultNormal = std::make_shared<Texture>("Textures\\DefaultNormalMap.png");
			m_DefaultNormal->SetType("texture_normalMap");

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
			const aiScene* scene = import.ReadFile(filePath.string(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

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
				//Vertex Positions
				glm::vec3 vector;
				vector.x = mesh->mVertices[i].x;
				vector.y = mesh->mVertices[i].y;
				vector.z = mesh->mVertices[i].z;
				vertex.Position = vector;

				//Normals
				vector.x = mesh->mNormals[i].x;
				vector.y = mesh->mNormals[i].y;
				vector.z = mesh->mNormals[i].z;
				vertex.Normal = vector;

				//UV
				if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
				{
					glm::vec2 vector;
					vector.x = mesh->mTextureCoords[0][i].x;
					vector.y = mesh->mTextureCoords[0][i].y;
					vertex.TexCoords = vector;
				}
				else
					vertex.TexCoords = glm::vec2(0.0f, 0.0f);	
			
				//TODO: Handle the case where the tangents cannot be calculated for the loaded model. That case would be models that don't have UV coordinates.
				//Tangnet
				vector.x = mesh->mTangents[i].x;
				vector.y = mesh->mTangents[i].y;
				vector.z = mesh->mTangents[i].z;
				vertex.Tangent = vector;

				//Biangnet
				vector.x = mesh->mBitangents[i].x;
				vector.y = mesh->mBitangents[i].y;
				vector.z = mesh->mBitangents[i].z;
				vertex.Bitangent = vector;

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

				aiTextureType type;
				if (m_FileType == "obj" || m_FileType == "OBJ")
					type = aiTextureType_HEIGHT;
				else
					type = aiTextureType_NORMALS;

				std::vector<Ref<Texture>> normalMaps;
				normalMaps = LoadMaterialTextures(material, type, "texture_normalMap");
				textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
			}
			return Mesh(vertices, indices, textures);
		}
		std::vector<Ref<Texture>> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName)
		{
			std::vector<Ref<Texture>> textures;
			std::string folderName = m_Directory.substr(m_Directory.find_last_of("\\/") + 1, m_Directory.length());
			for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
			{
				aiString str;
				mat->GetTexture(type, i, &str);
				std::string texturePath(str.C_Str());
				//Replace the following characters to convert the path into a standart one that uses "\\" by default.
				std::replace(texturePath.begin(), texturePath.end(), '/', '\\');
				std::string relativePath = "Models\\" + folderName + "\\" + texturePath;
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
					
					Ref<Texture> texture = std::make_shared<Texture>("Models\\" + folderName + "\\" + texturePath);
					texture->SetType(typeName);
					textures.push_back(texture);
					m_Textures.push_back(texture); // add to loaded textures
				}
			}
			//Handles the case which the loaded model doesnt contain the specific texture image. In that case, we send the default textures instead.
			if (textures.empty())
			{
				if (typeName == "texture_diffuse")
					textures.push_back(m_DefaultDiffuse);
				else if (typeName == "texture_specular")
					textures.push_back(m_DefaultSpecular);
				else if (typeName == "texture_normalMap")
					textures.push_back(m_DefaultNormal);
			}
			return textures;
		}
	}
}
