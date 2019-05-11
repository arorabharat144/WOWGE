#include "Graphics\Src\Precompiled.h"

#include "SceneStuff\SceneManager.h"
#include "SceneStuff\ModelNode.h"

using namespace WOWGE;
using namespace WOWGE::Graphics;

ModelNode::ModelNode()
{
	
}

ModelNode::~ModelNode()
{
	
}

Model& ModelNode::GetModel()
{
	return mModel;
}

TransformNode& ModelNode::GetTransformNode() const
{
	return *mTransformNode;
}

void ModelNode::InitializeSubNodes(SceneManager* sceneManager)
{
	mMeshNodes = new MeshNode*[mModel.GetNumberOfMeshes()];
	mMaterialNodes = new MaterialNode*[mModel.GetNumberOfMaterials()];

	mTransformNode = sceneManager->CreateTransformNode();
	mTransformNode->SetName("ModelTransform");
	mTransformNode->GetTransform().SetPosition(mModel.GetTransform().GetPosition());
	mTransformNode->GetTransform().SetRotation(mModel.GetTransform().GetRotation());
	mTransformNode->GetTransform().SetScale(mModel.GetTransform().GetScale());

	for (uint32_t i = 0; i < mModel.GetNumberOfMeshes(); ++i)
	{
		mMeshNodes[i] = sceneManager->CreateMeshNode();
		mMeshNodes[i]->SetName("ModelMesh");
		mMeshNodes[i]->SetMesh(&mModel.GetMeshBuffer(i));
	}

	for (uint32_t i{ 0 }; i < mModel.GetNumberOfMaterials(); ++i)
	{
		mMaterialNodes[i] = sceneManager->CreateMaterialNode();
		mMaterialNodes[i]->SetName("ModelMaterial");
		mMaterialNodes[i]->GetMaterial() = mModel.GetMaterial(i);
		if (mMaterialNodes[i]->GetMaterial().power == 0.0f)
		{
			mMaterialNodes[i]->GetMaterial().power = 10.0f;
		}

		mHasTexturesNode.push_back(sceneManager->CreateConstantBufferNode<HasTextureBuffer>(4));
	}

	for (auto texture : mModel.GetDiffuseTexturePaths())
	{
		mHasTexturesNode[texture.first]->GetData().hasTextures.x = 1.0f;
		mDiffuseTextureNodes[texture.first] = sceneManager->CreateTextureNode(ShaderStage::PixelShader);
		mDiffuseTextureNodes[texture.first]->SetName("ModelDiffuseTexture");

		mDiffuseTextureNodes[texture.first]->SetIndex(0);

		mDiffuseTextureNodes[texture.first]->SetTexture(&mModel.GetDiffuseTextures()[texture.second]);
	}

	for (auto texture : mModel.GetNormalTexturePaths())
	{
		mHasTexturesNode[texture.first]->GetData().hasTextures.y = 1.0f;
		mNormalTextureNodes[texture.first] = sceneManager->CreateTextureNode(ShaderStage::PixelShader);
		mNormalTextureNodes[texture.first]->SetName("ModelNormalTexture");

		mNormalTextureNodes[texture.first]->SetIndex(1);

		mNormalTextureNodes[texture.first]->SetTexture(&mModel.GetNormalTextures()[texture.second]);
	}

	for (auto texture : mModel.GetSpecularTexturePaths())
	{
		mHasTexturesNode[texture.first]->GetData().hasTextures.z = 1.0f;
		mSpecularTextureNodes[texture.first] = sceneManager->CreateTextureNode(ShaderStage::PixelShader);
		mSpecularTextureNodes[texture.first]->SetName("ModelSpecularTexture");

		mSpecularTextureNodes[texture.first]->SetIndex(2);

		mSpecularTextureNodes[texture.first]->SetTexture(&mModel.GetSpecularTextures()[texture.second]);
	}

	for (auto texture : mModel.GetDisplacementTexturePaths())
	{
		mHasTexturesNode[texture.first]->GetData().hasTextures.w = 1.0f;
		mDisplacementTextureNodes[texture.first] = sceneManager->CreateTextureNode(ShaderStage::VertexShader);
		mDisplacementTextureNodes[texture.first]->SetName("ModelDisplacementTexture");

		mDisplacementTextureNodes[texture.first]->SetIndex(3);

		mDisplacementTextureNodes[texture.first]->SetTexture(&mModel.GetDisplacementTextures()[texture.second]);
	}

	mBoneWorldTransformsNode = sceneManager->CreateConstantBufferNode<BoneBuffer>(5);
	mBoneWorldTransformsNode->SetName("BoneWorldTransforms");

	AddChild(mBoneWorldTransformsNode);
	mBoneWorldTransformsNode->AddChild(mTransformNode);

	for (uint32_t i = 0; i < mModel.GetNumberOfMaterials(); ++i)
	{
		mTransformNode->AddChild(mMaterialNodes[i]);
		mMaterialNodes[i]->AddChild(mHasTexturesNode[i]);

		auto diffuseTextureNode{ mDiffuseTextureNodes.find(i) };
		auto normalTextureNode{ mNormalTextureNodes.find(i) };
		auto specularTextureNode{ mSpecularTextureNodes.find(i) };
		auto displacementTextureNode{ mDisplacementTextureNodes.find(i) };

		SceneNode* sceneNode = mHasTexturesNode[i];

		if (diffuseTextureNode != mDiffuseTextureNodes.end() ||
			normalTextureNode != mNormalTextureNodes.end() ||
			specularTextureNode != mSpecularTextureNodes.end() ||
			displacementTextureNode != mDisplacementTextureNodes.end())
		{
			if (diffuseTextureNode != mDiffuseTextureNodes.end())
			{
				sceneNode->AddChild(mDiffuseTextureNodes[i]);
				sceneNode = mDiffuseTextureNodes[i];
			}
			if (normalTextureNode != mNormalTextureNodes.end())
			{
				sceneNode->AddChild(mNormalTextureNodes[i]);
				sceneNode = mNormalTextureNodes[i];
			}
			if (specularTextureNode != mSpecularTextureNodes.end())
			{
				sceneNode->AddChild(mSpecularTextureNodes[i]);
				sceneNode = mSpecularTextureNodes[i];
			
			}
			if (displacementTextureNode != mDisplacementTextureNodes.end())
			{
				sceneNode->AddChild(mDisplacementTextureNodes[i]);
				sceneNode = mDisplacementTextureNodes[i];
				
			}
			for (uint32_t j{ 0 }; j < mModel.GetMaterialIndices().size(); ++j)
			{
				if (mModel.GetMaterialIndices()[j] == i)
				{
					sceneNode->AddChild(mMeshNodes[j]);
				}
			}

		}
		else
		{
			for (uint32_t j{ 0 }; j < mModel.GetMaterialIndices().size(); ++j)
			{
				if (mModel.GetMaterialIndices()[j] == i)
				{
					sceneNode->AddChild(mMeshNodes[j]);
				}
			}
		}
	}
}

void ModelNode::OnUpdate(float deltaTime)
{
	for (uint32_t i{ 0 }; i < mModel.GetNumberOfBones(); ++i)
	{
		mBoneWorldTransformsNode->GetData().boneTransforms[i] = Maths::Transpose(mModel.GetBones()[i]->offsetTransform * mModel.GetBoneWorldTransforms()[i]);
	}
}

void ModelNode::Terminate()
{
	mModel.Terminate();
	DestroySubNodes();
	mDiffuseTextureNodes.clear();
}

void ModelNode::DestroySubNodes()
{
	SafeDeleteArray(mMeshNodes);
	SafeDeleteArray(mMaterialNodes);
}
