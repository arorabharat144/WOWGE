#ifndef INCLUDED_GRAPHICS_MODEL_H
#define INCLUDED_GRAPHICS_MODEL_H

#include "SkinnedMesh.h"
#include "MeshBuffer.h"
#include "Material.h"
#include "Texture.h"
#include "Transform.h"
#include "Bone.h"

#include "SceneStuff/MeshNode.h"
#include "SceneStuff/MaterialNode.h"
#include "SceneStuff/TransformNode.h"

namespace WOWGE
{
namespace Graphics
{

class Model
{
public:
	Model();
	~Model();

	void Terminate();

	uint32_t& GetNumberOfMeshes();
	uint32_t& GetNumberOfMaterials();

	SkinnedMesh& GetMesh(uint32_t index) const;

	SkinnedMesh*& GetMeshes();

	MeshBuffer& GetMeshBuffer(uint32_t index) const;

	Material*& GetMaterials();
	Material& GetMaterial(uint32_t index) const;

	std::vector<uint32_t>& GetMaterialIndices();

	Texture& GetDiffuseTexture(std::string str);
	const Texture& GetDiffuseTexture(std::string str) const;

	Bone*& GetRootBone();
	uint32_t& GetNumberOfBones();
	std::vector<Bone*>& GetBones();
	std::vector<Maths::Matrix44>& GetBoneWorldTransforms();

	std::map<std::string, Texture>& GetDiffuseTextures();
	std::map<uint32_t, std::string>& GetDiffuseTexturePaths();
	uint32_t& GetNumberOfDiffuseTextures();

	std::map<std::string, Texture>& GetNormalTextures();
	std::map<uint32_t, std::string>& GetNormalTexturePaths();
	uint32_t& GetNumberOfNormalTextures();

	std::map<std::string, Texture>& GetSpecularTextures();
	std::map<uint32_t, std::string>& GetSpecularTexturePaths();
	uint32_t& GetNumberOfSpecularTextures();

	std::map<std::string, Texture>& GetDisplacementTextures();
	std::map<uint32_t, std::string>& GetDisplacementTexturePaths();
	uint32_t& GetNumberOfDisplacementTextures();

	Transform& GetTransform();

	void Serialize(std::ofstream& outputFile);

	void LoadModelAscii(const char* fileName);

	void LoadModelBinary(const char* fileName);
	void LoadModelBinaryWithBones(const char* fileName);

private:
	void Deserialize(std::ifstream& inputFile);
	void DeserializeWithBones(std::ifstream& inputFile);

	void ResolveBoneLinks();

private:

	SkinnedMesh* mMeshes;
	MeshBuffer* mMeshBuffers;

	uint32_t mNumberOfMeshes;
	uint32_t mNumberOfMaterials;

	Material* mMaterials;

	std::vector<uint32_t> mMaterialIndices;
	
	std::map<std::string, Texture> mDiffuseTextures;
	std::map<uint32_t, std::string> mDiffuseTexturePaths;
	uint32_t mNumberOfDiffuseTextures;

	std::map<std::string, Texture> mNormalTextures;
	std::map<uint32_t, std::string> mNormalTexturePaths;
	uint32_t mNumberOfNormalTextures;
	
	std::map<std::string, Texture> mSpecularTextures;
	std::map<uint32_t, std::string> mSpecularTexturePaths;
	uint32_t mNumberOfSpecularTextures;
	
	std::map<std::string, Texture> mDisplacementTextures;
	std::map<uint32_t, std::string> mDisplacementTexturePaths;
	uint32_t mNumberOfDisplacementTextures;
	
	Transform mTransform;

	//Bones
	Bone* mRoot;
	std::vector<Bone*> mBones;
	std::vector<Maths::Matrix44> mBoneWorldTransforms;
	uint32_t mNumberOfBones;
};

} //namespace Graphics
} //namespace WOWGE

#endif // !INCLUDED_GRAPHICS_MODEL_H

