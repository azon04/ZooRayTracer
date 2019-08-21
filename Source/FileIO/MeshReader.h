#ifndef  _MESH_READER_H_
#define _MESH_READER_H_

#include "Mesh.h"

#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/Importer.hpp"

class MeshReader
{
public:
	MeshReader()
	{}

	Mesh* parse(const char* filename, float scale = 1.0f);

protected:
	Mesh* processNode(aiNode* node, const aiScene* scene, float scale);
};

Mesh* MeshReader::parse(const char* filename, float scale)
{
	Mesh* _result = nullptr;
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		return _result;
	}

	_result = processNode(scene->mRootNode, scene, scale);

	return _result;
}

Mesh* MeshReader::processNode(aiNode* node, const aiScene* scene, float scale)
{
	Mesh* result = nullptr;

	// Get first mesh #TODO process all meshes inside the file
	if (node->mNumMeshes > 0)
	{
		aiMesh* _mesh = scene->mMeshes[node->mMeshes[0]];
		result = new Mesh(_mesh->mNumFaces);
		for (int i = 0; i < _mesh->mNumFaces; i++)
		{
			aiFace& face = _mesh->mFaces[i];
			if (face.mNumIndices > 3)
			{
				continue;
			}

			Vec3 points[3];
			for (int j = 0; j < face.mNumIndices; j++)
			{
				unsigned int faceIndex = face.mIndices[j];
				points[j] = Vec3(_mesh->mVertices[faceIndex].x * scale, _mesh->mVertices[faceIndex].y * scale, _mesh->mVertices[faceIndex].z * scale);
			}

			Triangle* faceTriangle = new Triangle(points[0], points[1], points[2], nullptr);
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
