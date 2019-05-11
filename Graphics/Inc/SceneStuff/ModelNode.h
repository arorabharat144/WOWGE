#ifndef INCLUDED_GRAPHICS_MODELNODE_H
#define INCLUDED_GRAPHICS_MODELNODE_H

#include "Model.h"
#include "SceneNode.h"

#include "SceneStuff\ConstantBufferNode.h"
#include "SceneStuff\MeshNode.h"
#include "SceneStuff\MaterialNode.h"
#include "SceneStuff\TextureNode.h"
#include "SceneStuff\TransformNode.h"


namespace WOWGE
{
namespace Graphics
{

class SceneManager;

class ModelNode : public SceneNode
{
public:
	ModelNode();
	~ModelNode();

	Model& GetModel();
	TransformNode& GetTransformNode() const;

	void InitializeSubNodes(SceneManager* sceneManager);

private:
	void DestroySubNodes();
private:
	void OnUpdate(float deltaTime) override;
	void Terminate() override;
private:

	struct HasTextureBuffer
	{
		Maths::Vector4 hasTextures;
	};

	struct BoneBuffer
	{
		WOWGE::Maths::Matrix44 boneTransforms[256];
	};

	ConstantBufferNode<BoneBuffer>* mBoneWorldTransformsNode;

	std::vector<ConstantBufferNode<HasTextureBuffer>*> mHasTexturesNode;

	Model mModel;

	MeshNode** mMeshNodes;
	MaterialNode** mMaterialNodes;

	std::map<uint32_t, TextureNode*> mDiffuseTextureNodes;
	std::map<uint32_t, TextureNode*> mNormalTextureNodes;
	std::map<uint32_t, TextureNode*> mSpecularTextureNodes;
	std::map<uint32_t, TextureNode*> mDisplacementTextureNodes;

	TransformNode* mTransformNode;
};

} //namespace Graphics
} //namespace WOWGE

#endif // !INCLUDED_GRAPHICS_MODELNODE_H
