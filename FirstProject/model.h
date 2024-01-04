#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>
#include <vector>
#include <map>

#include "shader.h"
#include "mesh.h"

using namespace std;

class Model {
public:

	Model(const char* path) {
		loadModel(path);
	};

	void draw(Shader& shader) {
		for (Mesh mesh : meshes) {
			mesh.draw(shader);
		}
	};

private:
	vector<Mesh> meshes;
	string directory;
	map<string, unsigned int> loadedTextures;
	void loadModel(string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	vector<Texture> loadMaterials(aiMaterial* mat, aiTextureType type, string typeName);
	unsigned int readTextureFromFile(const char* path, string directory);
};

void Model::loadModel(string path) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		cout << "assimp loading error" << importer.GetErrorString() << endl;
	}

	directory = path.substr(0, path.find_last_of('/'));

	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene) {
	for (int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}

	for (int i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;
	
	// vertices
	for (int i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;
		vertex.Position.x = mesh->mVertices[i].x;
		vertex.Position.y = mesh->mVertices[i].y;
		vertex.Position.z = mesh->mVertices[i].z;

		vertex.Normal.x = mesh->mNormals[i].x;
		vertex.Normal.y = mesh->mNormals[i].y;
		vertex.Normal.z = mesh->mNormals[i].z;

		if (mesh->mTextureCoords[0]) {
			vertex.TexCoord.x = mesh->mTextureCoords[0][i].x;
			vertex.TexCoord.y = mesh->mTextureCoords[0][i].y;
		}
		vertices.push_back(vertex);
	}

	// indices
	for (int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (int j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}

	// textures
	if (mesh->mMaterialIndex >= 0) {
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		vector<Texture> diffuseMaps = loadMaterials(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		vector<Texture> specularMaps = loadMaterials(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	return Mesh(vertices, indices, textures);
}

vector<Texture> Model::loadMaterials(aiMaterial* mat, aiTextureType type, string typeName) {
	vector<Texture> textures;
	for (int i = 0; i < mat->GetTextureCount(type); i++) {
		aiString str;
		mat->GetTexture(type, i, &str);
		string name = string(str.C_Str());
		Texture texture;
		if (loadedTextures.find(name) != loadedTextures.end()) {
			texture.id = loadedTextures[name];
		}
		else {
			texture.id = readTextureFromFile(name.c_str(), directory);
			loadedTextures[name] = texture.id;
		}
		texture.type = typeName;
		// texture.path = str;
		textures.push_back(texture);
	}
	return textures;
}

unsigned int Model::readTextureFromFile(const char* path, string directory) {
	string fileName = string(path);
	fileName = directory + '/' + fileName;

	unsigned int texture;
	glGenTextures(1, &texture);
	// bind object, set target for following operation
	glBindTexture(GL_TEXTURE_2D, texture);

	int width, height, nChannel;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(fileName.c_str(), &width, &height, &nChannel, 0);

	if (data) {
		GLenum format;
		if (nChannel == 1) {
			format = GL_RED;
		}
		else if (nChannel == 3) {
			format = GL_RGB;
		}
		else if (nChannel == 4) {
			format = GL_RGBA;
		}
		else {
			std::cout << "unknown image format" << std::endl;
			return -1;
		}

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);
	}
	else {
		std::cout << "fail to load image" << std::endl;
		return -1;
	}

	return texture;
}
