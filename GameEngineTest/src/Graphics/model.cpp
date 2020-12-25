#include "model.h"

namespace GameEngineTest {
	namespace Graphics {

		void Model::Draw(Shader& shader , glm::mat4 camera)
		{
			for (unsigned int i = 0; i < meshes.size(); i++)
				meshes[i].Draw(shader, camera);
		}
		void Model::loadModel(std::string path)
		{
			Assimp::Importer import;
			const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

			if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
			{
				std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
				return;
			}
			directory = path.substr(0, path.find_last_of('/'));
			processNode(scene->mRootNode, scene);
		}
		void Model::processNode(aiNode* node, const aiScene* scene)
		{
			// process all the node's meshes (if any)
			for (unsigned int i = 0; i < node->mNumMeshes; i++)
			{
				aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
				vertexCount += mesh->mNumVertices;
				meshes.push_back(processMesh(mesh, scene));
			}
			// then do the same for each of its children
			for (unsigned int i = 0; i < node->mNumChildren; i++)
			{
				std::cout << node->mChildren[i]->mName.C_Str() << std::endl;
				processNode(node->mChildren[i], scene);
			}
		}
		Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
		{
			std::vector<Vertex> vertices;
			//std::vector<unsigned int> indices;
			//std::vector<Texture> textures;

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
					glm::vec2 vec;
					vec.x = mesh->mTextureCoords[0][i].x;
					vec.y = mesh->mTextureCoords[0][i].y;
					vertex.TexCoords = vec;
				}
				else
					vertex.TexCoords = glm::vec2(0.0f, 0.0f);

				//indices.push_back(0);
				//textures.push_back(Texture("H:\\ProgrammingProjects\\repos\\GameEngineTest\\GameEngineTest\\textures\\tileable-grass_clover_tt7010116-sample_1.png"));
				vertices.push_back(vertex);

			}
			// process indices

			// process material
			if (mesh->mMaterialIndex >= 0)
			{
			}

			return Mesh(vertices);
		}
		//std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
		//{
		//	
		//}
		//
	}
}
