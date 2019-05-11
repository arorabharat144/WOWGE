#include "Precompiled.h"
#include "Model.h"

#include "SceneStuff/SceneManager.h"

#include <string>
#include <fstream>
#include <iostream>

using namespace WOWGE;
using namespace WOWGE::Graphics;

Model::Model() :
	mMeshes(nullptr),
	mMeshBuffers(nullptr),
	mNumberOfMeshes(0),
	mMaterials(nullptr),
	//mTextures(nullptr),
	mNumberOfDiffuseTextures(0)
{

}

Model::~Model()
{

}

void Model::Terminate()
{
	for (uint32_t i = 0; i < mNumberOfMeshes; ++i)
	{
		mMeshes[i].Destroy();
		mMeshBuffers[i].Terminate();
	}

	for (auto texture : mDiffuseTexturePaths)
	{
		if (mDiffuseTextures.find(texture.second) != mDiffuseTextures.end())
		{
			mDiffuseTextures[texture.second].Terminate();
		}
	}

	for (auto texture : mNormalTexturePaths)
	{
		if (mNormalTextures.find(texture.second) != mNormalTextures.end())
		{
			mNormalTextures[texture.second].Terminate();
		}
	}

	for (auto texture : mSpecularTexturePaths)
	{
		if (mSpecularTextures.find(texture.second) != mSpecularTextures.end())
		{
			mSpecularTextures[texture.second].Terminate();
		}
	}

	for (auto texture : mDisplacementTexturePaths)
	{
		if (mDisplacementTextures.find(texture.second) != mDisplacementTextures.end())
		{
			mDisplacementTextures[texture.second].Terminate();
		}
	}

	mDiffuseTextures.clear();
	mDiffuseTexturePaths.clear();

	mNormalTextures.clear();
	mNormalTexturePaths.clear();

	mSpecularTextures.clear();
	mSpecularTexturePaths.clear();

	mDisplacementTextures.clear();
	mDisplacementTexturePaths.clear();

	SafeDeleteArray(mMeshes);
	SafeDeleteArray(mMeshBuffers);
	SafeDeleteArray(mMaterials);
	//SafeDeleteArray(mTextures);
	for (uint32_t i{ 0 }; i < mBones.size(); ++i)
	{
		SafeDelete(mBones[i]);
	}
}

uint32_t& Model::GetNumberOfMeshes()
{
	return mNumberOfMeshes;
}

uint32_t& Model::GetNumberOfMaterials()
{
	return mNumberOfMaterials;
}

SkinnedMesh& Model::GetMesh(uint32_t index) const
{
	return mMeshes[index];
}

SkinnedMesh*& Model::GetMeshes()
{
	return mMeshes;
}

MeshBuffer& Model::GetMeshBuffer(uint32_t index) const
{
	return mMeshBuffers[index];
}

Material*& Model::GetMaterials()
{
	return mMaterials;
}

Material& Model::GetMaterial(uint32_t index) const
{
	return mMaterials[index];
}

std::vector<uint32_t>& Model::GetMaterialIndices()
{
	return mMaterialIndices;
}

Texture& Model::GetDiffuseTexture(std::string str)
{
	return mDiffuseTextures[str];
}

const Texture& Model::GetDiffuseTexture(std::string str) const
{
	return mDiffuseTextures.at(str);
}

Bone*& Model::GetRootBone()
{
	return mRoot;
}

uint32_t& Model::GetNumberOfBones()
{
	return mNumberOfBones;
}

std::vector<Bone*>& Model::GetBones()
{
	return mBones;
}

std::vector<Maths::Matrix44>& Model::GetBoneWorldTransforms()
{
	return mBoneWorldTransforms;
}

//Bone*& Model::GetBone(uint32_t index)
//{
//	return mBones[index];
//}

std::map<std::string, Texture>& Model::GetDiffuseTextures()
{
	return mDiffuseTextures;
}

std::map<uint32_t, std::string>& Model::GetDiffuseTexturePaths()
{
	return mDiffuseTexturePaths;
}

uint32_t& Model::GetNumberOfDiffuseTextures()
{
	return mNumberOfDiffuseTextures;
}

std::map<std::string, Texture>& Model::GetNormalTextures()
{
	return mNormalTextures;
}

std::map<uint32_t, std::string>& Model::GetNormalTexturePaths()
{
	return mNormalTexturePaths;
}

uint32_t& Model::GetNumberOfNormalTextures()
{
	return mNumberOfNormalTextures;
}

std::map<std::string, Texture>& Model::GetSpecularTextures()
{
	return mSpecularTextures;
}

std::map<uint32_t, std::string>& Model::GetSpecularTexturePaths()
{
	return mSpecularTexturePaths;
}

uint32_t& Model::GetNumberOfSpecularTextures()
{
	return mNumberOfSpecularTextures;
}

std::map<std::string, Texture>& Model::GetDisplacementTextures()
{
	return mDisplacementTextures;
}

std::map<uint32_t, std::string>& Model::GetDisplacementTexturePaths()
{
	return mDisplacementTexturePaths;
}

uint32_t& Model::GetNumberOfDisplacementTextures()
{
	return mNumberOfDisplacementTextures;
}

Transform& Model::GetTransform()
{
	return mTransform;
}

void Model::Serialize(std::ofstream& outputFile)
{
	outputFile.write((char*)&mNumberOfMeshes, sizeof(uint32_t));
	outputFile.write((char*)&mNumberOfMaterials, sizeof(uint32_t));
	outputFile.write((char*)&mNumberOfBones, sizeof(uint32_t));

	uint32_t vertexCount;
	uint32_t indexCount;

	uint32_t materialIndicesCount{ static_cast<uint32_t>(mMaterialIndices.size()) };
	outputFile.write((char*)&materialIndicesCount, sizeof(uint32_t));

	outputFile.write((char*)&mNumberOfDiffuseTextures, sizeof(uint32_t));
	outputFile.write((char*)&mNumberOfNormalTextures, sizeof(uint32_t));
	outputFile.write((char*)&mNumberOfSpecularTextures, sizeof(uint32_t));
	outputFile.write((char*)&mNumberOfDisplacementTextures, sizeof(uint32_t));

	for (uint32_t i = 0; i < mNumberOfMeshes; ++i)
	{
		vertexCount = mMeshes[i].GetVertexCount();
		indexCount = mMeshes[i].GetIndexCount();
		outputFile.write((char*)&vertexCount, sizeof(uint32_t));
		outputFile.write((char*)&indexCount, sizeof(uint32_t));

		for (uint32_t j = 0; j < mMeshes[i].GetVertexCount(); ++j)
		{
			outputFile.write((char*)&mMeshes[i].GetVertex(j), sizeof(BoneVertex));
		}

		for (uint32_t j = 0; j < mMeshes[i].GetIndexCount(); ++j)
		{
			outputFile.write((char*)&mMeshes[i].GetIndex(j), sizeof(uint32_t));
		}

		outputFile.write((char*)&mMaterialIndices[i], sizeof(uint32_t));
	}

	for (uint32_t i{ 0 }; i < mNumberOfMaterials; ++i)
	{
		outputFile.write((char*)&mMaterials[i], sizeof(Material));
	}

	for (auto texture : mDiffuseTexturePaths)
	{
		outputFile.write((char*)&texture.first, sizeof(uint32_t));

		uint32_t size = static_cast<uint32_t>(texture.second.size());
		outputFile.write((char*)&size, sizeof(uint32_t));

		outputFile.write(texture.second.c_str(), size);
	}

	for (auto texture : mNormalTexturePaths)
	{
		outputFile.write((char*)&texture.first, sizeof(uint32_t));

		uint32_t size = static_cast<uint32_t>(texture.second.size());
		outputFile.write((char*)&size, sizeof(uint32_t));

		outputFile.write(texture.second.c_str(), size);
	}

	for (auto texture : mSpecularTexturePaths)
	{
		outputFile.write((char*)&texture.first, sizeof(uint32_t));

		uint32_t size = static_cast<uint32_t>(texture.second.size());
		outputFile.write((char*)&size, sizeof(uint32_t));

		outputFile.write(texture.second.c_str(), size);
	}

	for (auto texture : mDisplacementTexturePaths)
	{
		outputFile.write((char*)&texture.first, sizeof(uint32_t));

		uint32_t size = static_cast<uint32_t>(texture.second.size());
		outputFile.write((char*)&size, sizeof(uint32_t));

		outputFile.write(texture.second.c_str(), size);
	}

	for (uint32_t i{ 0 }; i < mNumberOfBones; ++i)
	{
		uint32_t boneNameStringSize{ static_cast<uint32_t>(mBones[i]->name.size()) };

		outputFile.write((char*)&boneNameStringSize, sizeof(uint32_t));
		outputFile.write(mBones[i]->name.c_str(), boneNameStringSize);

		outputFile.write((char*)&mBones[i]->transform, sizeof(Maths::Matrix44));
		outputFile.write((char*)&mBones[i]->offsetTransform, sizeof(Maths::Matrix44));

		outputFile.write((char*)&mBones[i]->index, sizeof(int));
		outputFile.write((char*)&mBones[i]->parentIndex, sizeof(int));

		uint32_t numberOfChildren{ static_cast<uint32_t>(mBones[i]->childrenIndex.size()) };
		outputFile.write((char*)&numberOfChildren, sizeof(size_t));
		for (uint32_t j{ 0 }; j < mBones[i]->childrenIndex.size(); ++j)
		{
			outputFile.write((char*)&mBones[i]->childrenIndex[j], sizeof(int));
		}
	}
}

//Without textures i.e. incomplete
void Model::LoadModelAscii(const char* fileName)
{
	std::ifstream inputFile;
	std::string line;
	inputFile.open(fileName);

	inputFile >> line >> line >> line; //Number of meshes:

	inputFile >> mNumberOfMeshes;

	mMeshes = new SkinnedMesh[mNumberOfMeshes];

	//inputFile >> mNumberOfMaterials;
	mMaterials = new Material[mNumberOfMeshes];

	uint32_t numberOfVertices;
	uint32_t numberOfIndices;
	for (uint32_t meshIndex = 0; meshIndex < mNumberOfMeshes; ++meshIndex)
	{
		inputFile >> line >> line >> line; //Number of vertices:
		inputFile >> numberOfVertices;

		std::cout << numberOfVertices;

		inputFile >> line >> line >> line; //Number of indices:
		inputFile >> numberOfIndices;

		std::cout << numberOfIndices;

		mMeshes[meshIndex].Allocate(numberOfVertices, numberOfIndices);

		inputFile >> line; //Positions: 
		for (uint32_t i = 0; i < numberOfVertices; ++i)
		{
			inputFile >> mMeshes[meshIndex].GetVertex(i).position.x >> mMeshes[meshIndex].GetVertex(i).position.y >> mMeshes[meshIndex].GetVertex(i).position.z;
		}

		inputFile >> line; //Tangents:
		if (line.compare("Tangents:NO") != 0)
		{
			for (uint32_t i = 0; i < numberOfVertices; ++i)
			{
				inputFile >> mMeshes[meshIndex].GetVertex(i).tangent.x >> mMeshes[meshIndex].GetVertex(i).tangent.y >> mMeshes[meshIndex].GetVertex(i).tangent.z;
			}
		}

		inputFile >> line; //UVs:
		if (line.compare("UVs:NO") != 0)
		{
			for (uint32_t i = 0; i < numberOfVertices; ++i)
			{
				inputFile >> mMeshes[meshIndex].GetVertex(i).uv.x >> mMeshes[meshIndex].GetVertex(i).uv.y;
			}
		}

		inputFile >> line; //Indices:
		for (uint32_t i = 0; i < numberOfIndices; i += 3)
		{
			inputFile >> mMeshes[meshIndex].GetIndex(i) >> mMeshes[meshIndex].GetIndex(i + 1) >> mMeshes[meshIndex].GetIndex(i + 2);
		}

		inputFile >> line; //Normals:
		for (uint32_t i = 0; i < numberOfVertices; ++i)
		{
			inputFile >> mMeshes[meshIndex].GetVertex(i).normal.x >> mMeshes[meshIndex].GetVertex(i).normal.y >> mMeshes[meshIndex].GetVertex(i).normal.z;
		}

		inputFile >> line; //Materials:

		inputFile >> line; //Ambient:
		inputFile >> mMaterials[meshIndex].ambient.x >> mMaterials[meshIndex].ambient.y >> mMaterials[meshIndex].ambient.z >> mMaterials[meshIndex].ambient.w;

		inputFile >> line; //Diffuse
		inputFile >> mMaterials[meshIndex].diffuse.x >> mMaterials[meshIndex].diffuse.y >> mMaterials[meshIndex].diffuse.z >> mMaterials[meshIndex].diffuse.w;

		inputFile >> line; //Specular
		inputFile >> mMaterials[meshIndex].specular.x >> mMaterials[meshIndex].specular.y >> mMaterials[meshIndex].specular.z >> mMaterials[meshIndex].specular.w;

		inputFile >> line; //Emissive
		inputFile >> mMaterials[meshIndex].emissive.x >> mMaterials[meshIndex].emissive.y >> mMaterials[meshIndex].emissive.z >> mMaterials[meshIndex].emissive.w;

		inputFile >> line; //Power
		inputFile >> mMaterials[meshIndex].power;
	}

	inputFile.close();
}

//Probably wont work, since didn't update with the model importer
void Model::LoadModelBinary(const char* fileName)
{
	std::ifstream inputFile;
	inputFile.open(fileName, std::ios::in | std::ios::binary);

	Deserialize(inputFile);

	mMeshBuffers = new MeshBuffer[mNumberOfMeshes];
	for (uint32_t i = 0; i < mNumberOfMeshes; ++i)
	{
		mMeshBuffers[i].Initialize(mMeshes[i]);
	}
}

void Model::LoadModelBinaryWithBones(const char* fileName)
{
	std::ifstream inputFile;
	inputFile.open(fileName, std::ios::in | std::ios::binary);

	DeserializeWithBones(inputFile);

	mMeshBuffers = new MeshBuffer[mNumberOfMeshes];
	for (uint32_t i = 0; i < mNumberOfMeshes; ++i)
	{
		mMeshBuffers[i].Initialize(mMeshes[i]);
	}

	for (auto texture : mDiffuseTexturePaths)
	{
		if (mDiffuseTextures.find(texture.second) == mDiffuseTextures.end())
		{
			mDiffuseTextures[texture.second].Initialize(texture.second.c_str());
		}
	}

	for (auto texture : mNormalTexturePaths)
	{
		if (mNormalTextures.find(texture.second) == mNormalTextures.end())
		{
			mNormalTextures[texture.second].Initialize(texture.second.c_str());
		}
	}

	for (auto texture : mSpecularTexturePaths)
	{
		if (mSpecularTextures.find(texture.second) == mSpecularTextures.end())
		{
			mSpecularTextures[texture.second].Initialize(texture.second.c_str());
		}
	}

	for (auto texture : mDisplacementTexturePaths)
	{
		if (mDisplacementTextures.find(texture.second) == mDisplacementTextures.end())
		{
			mDisplacementTextures[texture.second].Initialize(texture.second.c_str());
		}
	}

	ResolveBoneLinks();

	mBoneWorldTransforms.resize(mNumberOfBones);
}

void Model::Deserialize(std::ifstream& inputFile)
{
	inputFile.read((char*)&mNumberOfMeshes, sizeof(uint32_t));
	inputFile.read((char*)&mNumberOfMaterials, sizeof(uint32_t));

	mMeshes = new SkinnedMesh[mNumberOfMeshes];
	mMaterials = new Material[mNumberOfMaterials];
	uint32_t vertexCount;
	uint32_t indexCount;

	inputFile.read((char*)&mNumberOfDiffuseTextures, sizeof(uint32_t));
	for (uint32_t i = 0; i < mNumberOfMeshes; ++i)
	{
		inputFile.read((char*)&vertexCount, sizeof(uint32_t));
		inputFile.read((char*)&indexCount, sizeof(uint32_t));

		mMeshes[i].Allocate(vertexCount, indexCount);

		for (uint32_t j = 0; j < mMeshes[i].GetVertexCount(); ++j)
		{
			inputFile.read((char*)&mMeshes[i].GetVertex(j), sizeof(Vertex));
		}

		for (uint32_t j = 0; j < mMeshes[i].GetIndexCount(); ++j)
		{
			inputFile.read((char*)&mMeshes[i].GetIndex(j), sizeof(uint32_t));
		}
	}

	for (uint32_t i{ 0 }; i < mNumberOfMaterials; ++i)
	{
		inputFile.read((char*)&mMaterials[i], sizeof(Material));

		uint32_t textureCount;
		inputFile.read((char*)&textureCount, sizeof(uint32_t));

		if (textureCount > 0)
		{
			uint32_t stringSize;
			inputFile.read((char*)&stringSize, sizeof(uint32_t));

			char* temp = new char[stringSize + 1];
			inputFile.read(temp, stringSize);
			temp[stringSize] = '\0';
			mDiffuseTexturePaths[i] = temp;

			SafeDeleteArray(temp);
		}
	}
}

void Model::DeserializeWithBones(std::ifstream& inputFile)
{
	inputFile.read((char*)&mNumberOfMeshes, sizeof(uint32_t));
	inputFile.read((char*)&mNumberOfMaterials, sizeof(uint32_t));
	inputFile.read((char*)&mNumberOfBones, sizeof(uint32_t));

	mMeshes = new SkinnedMesh[mNumberOfMeshes];
	mMaterials = new Material[mNumberOfMaterials];

	uint32_t vertexCount;
	uint32_t indexCount;

	uint32_t numberOfMaterialIndices;
	inputFile.read((char*)&numberOfMaterialIndices, sizeof(uint32_t));

	mMaterialIndices.resize(numberOfMaterialIndices);

	inputFile.read((char*)&mNumberOfDiffuseTextures, sizeof(uint32_t));
	inputFile.read((char*)&mNumberOfNormalTextures, sizeof(uint32_t));
	inputFile.read((char*)&mNumberOfSpecularTextures, sizeof(uint32_t));
	inputFile.read((char*)&mNumberOfDisplacementTextures, sizeof(uint32_t));

	for (uint32_t i = 0; i < mNumberOfMeshes; ++i)
	{
		inputFile.read((char*)&vertexCount, sizeof(uint32_t));
		inputFile.read((char*)&indexCount, sizeof(uint32_t));

		mMeshes[i].Allocate(vertexCount, indexCount);

		for (uint32_t j = 0; j < mMeshes[i].GetVertexCount(); ++j)
		{
			inputFile.read((char*)&mMeshes[i].GetVertex(j), sizeof(BoneVertex));
		}

		for (uint32_t j = 0; j < mMeshes[i].GetIndexCount(); ++j)
		{
			inputFile.read((char*)&mMeshes[i].GetIndex(j), sizeof(uint32_t));
		}

		inputFile.read((char*)&mMaterialIndices[i], sizeof(uint32_t));
	}

	for (uint32_t i{ 0 }; i < mNumberOfMaterials; ++i)
	{
		inputFile.read((char*)&mMaterials[i], sizeof(Material));
	}

	for (uint32_t i{ 0 }; i < mNumberOfDiffuseTextures; ++i)
	{
		uint32_t materialIndex;
		inputFile.read((char*)&materialIndex, sizeof(uint32_t));

		uint32_t stringSize;
		inputFile.read((char*)&stringSize, sizeof(uint32_t));

		std::string texturePath;
		char* temp = new char[stringSize + 1];
		inputFile.read(temp, stringSize);
		temp[stringSize] = '\0';
		texturePath = temp;

		mDiffuseTexturePaths[materialIndex] = texturePath;

		SafeDeleteArray(temp);
	}

	for (uint32_t i{ 0 }; i < mNumberOfNormalTextures; ++i)
	{
		uint32_t materialIndex;
		inputFile.read((char*)&materialIndex, sizeof(uint32_t));

		uint32_t stringSize;
		inputFile.read((char*)&stringSize, sizeof(uint32_t));

		std::string texturePath;
		char* temp = new char[stringSize + 1];
		inputFile.read(temp, stringSize);
		temp[stringSize] = '\0';
		texturePath = temp;

		mNormalTexturePaths[materialIndex] = texturePath;

		SafeDeleteArray(temp);
	}

	for (uint32_t i{ 0 }; i < mNumberOfSpecularTextures; ++i)
	{
		uint32_t materialIndex;
		inputFile.read((char*)&materialIndex, sizeof(uint32_t));

		uint32_t stringSize;
		inputFile.read((char*)&stringSize, sizeof(uint32_t));

		std::string texturePath;
		char* temp = new char[stringSize + 1];
		inputFile.read(temp, stringSize);
		temp[stringSize] = '\0';
		texturePath = temp;

		mSpecularTexturePaths[materialIndex] = texturePath;

		SafeDeleteArray(temp);
	}

	for (uint32_t i{ 0 }; i < mNumberOfDisplacementTextures; ++i)
	{
		uint32_t materialIndex;
		inputFile.read((char*)&materialIndex, sizeof(uint32_t));

		uint32_t stringSize;
		inputFile.read((char*)&stringSize, sizeof(uint32_t));

		std::string texturePath;
		char* temp = new char[stringSize + 1];
		inputFile.read(temp, stringSize);
		temp[stringSize] = '\0';
		texturePath = temp;

		mDisplacementTexturePaths[materialIndex] = texturePath;

		SafeDeleteArray(temp);
	}

	for (uint32_t i{ 0 }; i < mNumberOfBones; ++i)
	{
		Bone* bone{ new Bone() };
		uint32_t boneNameStringSize;

		inputFile.read((char*)&boneNameStringSize, sizeof(uint32_t));

		std::unique_ptr<char[]> temp{ std::make_unique<char[]>(boneNameStringSize + 1) };
		inputFile.read((char*)temp.get(), boneNameStringSize);
		temp[boneNameStringSize] = '\0';

		bone->name = temp.get();

		inputFile.read((char*)&bone->transform, sizeof(Maths::Matrix44));
		inputFile.read((char*)&bone->offsetTransform, sizeof(Maths::Matrix44));

		inputFile.read((char*)&bone->index, sizeof(int));
		inputFile.read((char*)&bone->parentIndex, sizeof(int));

		uint32_t numberOfChildren;
		inputFile.read((char*)&numberOfChildren, sizeof(uint32_t));

		for (uint32_t j{ 0 }; j < numberOfChildren; ++j)
		{
			uint32_t childIndex;
			inputFile.read((char*)&childIndex, sizeof(int));
			bone->childrenIndex.emplace_back(childIndex);
		}

		mBones.emplace_back(bone);
	}
}

void Model::ResolveBoneLinks()
{
	for (uint32_t i{ 0 }; i < mNumberOfBones; ++i)
	{
		Bone* bone{ mBones[i] };
		if (bone->parentIndex == -1)
		{
			mRoot = bone;
		}
		else
		{
			bone->parent = mBones[bone->parentIndex];
		}

		if (!bone->childrenIndex.empty())
		{
			for (uint32_t j{ 0 }; j < bone->childrenIndex.size(); ++j)
			{
				bone->children.emplace_back(mBones[bone->childrenIndex[j]]);
			}
		}
	}
}