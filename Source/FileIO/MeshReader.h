#ifndef  _MESH_READER_H_
#define _MESH_READER_H_

#include "mesh.h"

#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/Importer.hpp"

class MeshReader
{
public:
	MeshReader()
	{}

	mesh* parse(const char* filename, float scale = 1.0f);

protected:
	mesh* processNode(aiNode* node, const aiScene* scene, float scale);
};

mesh* MeshReader::parse(const char* filename, float scale)
{
	mesh* _result = nullptr;
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		return _result;
	}

	_result = processNode(scene->mRootNode, scene, scale);

	return _result;
}

mesh* MeshReader::processNode(aiNode* node, const aiScene* scene, float scale)
{
	mesh* result = nullptr;

	// Get first mesh #TODO process all meshes inside the file
	if (node->mNumMeshes > 0)
	{
		aiMesh* _mesh = scene->mMeshes[node->mMeshes[0]];
		result = new mesh(_mesh->mNumFaces);
		for (int i = 0; i < _mesh->mNumFaces; i++)
		{
			aiFace& face = _mesh->mFaces[i];
			if (face.mNumIndices > 3)
			{
				continue;
			}

			vec3 points[3];
			for (int j = 0; j < face.mNumIndices; j++)
			{
				unsigned int faceIndex = face.mIndices[j];
				points[j] = vec3(_mesh->mVertices[faceIndex].x * scale, _mesh->mVertices[faceIndex].y * scale, _mesh->mVertices[faceIndex].z * scale);
			}

			triangle* faceTriangle = new triangle(points[0], points[1], points[2], nullptr);
			result->addTriangle(faceTriangle);
		}
	}
	else if(node->mNumChildren > 0)
	{
		result = processNode(node->mChildren[0], scene, scale);
	}

	if (result)
	{
		result->setupMesh();
	}

	return result;
}

#endif
