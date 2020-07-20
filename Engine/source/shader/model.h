#pragma once
#include <shader/shader.h>
#include <shader/mesh.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <precom.h>

enum Models {
	BACKPACK = 0,
};

class Model
{
public:
	Model(Models object)
	{
		std::string path;
		switch (object)
		{
		case BACKPACK:
			path = "D:/dev/0xdenmuell/GLFW-Engine2/Engine/source/shader/ressources/Backpack/backpack.obj";
			break;
		default:
			LOG("CANT FIND MODEL");
			break;
		}
		loadModel(path);
	}
	void Draw(Shader& shader)
	{
		for (unsigned int i = 0; i < meshes.size(); i++) {
			meshes[i].Draw(shader);
		}
	}
private:
	// model data
	std::vector<Mesh> meshes;
	std::string directory;
	std::vector<Texture> textures_loaded;


	void loadModel(std::string path) {

		//load data in importer
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

		//check for errors
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			LOG("ERROR::ASSIMP::" << importer.GetErrorString());
			return;
		}
		directory = path.substr(0, path.find_last_of('/'));
		processNode(scene->mRootNode, scene);
	}

	void processNode(aiNode* node, const aiScene* scene) {
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(processMesh(mesh, scene));
		}
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene);
		}
	}

	Mesh processMesh(aiMesh* mesh, const aiScene* scene) {

		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture> textures;

		//vertices
		for (unsigned int i = 0; i < mesh->mNumVertices; i++) 
		{
			Vertex vertex;

			glm::vec3 transferVector;
			transferVector.x = mesh->mVertices[i].x;
			transferVector.y = mesh->mVertices[i].y;
			transferVector.z = mesh->mVertices[i].z;

			vertex.Position = transferVector;

			transferVector.x = mesh->mNormals[i].x;
			transferVector.y = mesh->mNormals[i].y;
			transferVector.z = mesh->mNormals[i].z;

			vertex.Normal = transferVector;

			if (mesh->mTextureCoords[0])
			{
				glm::vec2 transferVectorTex;
				transferVectorTex.x = mesh->mTextureCoords[0][i].x;
				transferVectorTex.y = mesh->mTextureCoords[0][i].y;
				vertex.TexCoords = transferVectorTex;
			}
			else {
				vertex.TexCoords = glm::vec2(0.0f, 0.0f);
			}
			vertices.push_back(vertex);
		} 

		//indices
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++) {
				indices.push_back(face.mIndices[j]);
			}
		}

		//textures

			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

			std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

			std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	

		return Mesh(vertices, indices, textures);
	}
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName) {

		std::vector<Texture> textures;
		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);
			bool skip = false;
			for (unsigned int j = 0; j < textures_loaded.size(); j++)
			{
				if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
				{
					textures.push_back(textures_loaded[j]);
					skip = true;
					break;
				}
			}
			if (!skip)
			{
				Texture texture;
				texture.id = Shader::loadMaterialTexture(directory + "/" + str.C_Str());
				texture.type = typeName;
				texture.path = str.C_Str();
				textures.push_back(texture);
				textures_loaded.push_back(texture);
			}
		}
		return textures;
	}

};