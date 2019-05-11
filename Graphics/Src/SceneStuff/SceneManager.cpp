//====================================================================================================
// Filename:	SceneManager.cpp
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Graphics\Src\Precompiled.h"

#include "Graphics\Inc\SceneStuff\SceneManager.h"

using namespace WOWGE;
using namespace WOWGE::Graphics;

//====================================================================================================
// Class Definitions
//====================================================================================================

SceneManager::SceneManager(MatrixStack& ms)
	: mMatrixStack(ms)
{
	// Empty
}

//----------------------------------------------------------------------------------------------------

SceneManager::~SceneManager()
{
	ASSERT(mNodes.empty(), "[SceneManager] Purge() must be called before destruction.");
}

//----------------------------------------------------------------------------------------------------

CameraNode* SceneManager::CreateCameraNode()
{
	CameraNode* newNode = new CameraNode();
	newNode->Initialize();
	mNodes.push_back(newNode);
	return newNode;
}

//----------------------------------------------------------------------------------------------------
CameraNode* SceneManager::CreateCameraNode(uint32_t index)
{
	CameraNode* newNode = new CameraNode();
	newNode->Initialize(index);
	mNodes.push_back(newNode);
	return newNode;
}
//----------------------------------------------------------------------------------------------------
DepthMapNode* SceneManager::CreateDepthMapNode(uint32_t index)
{
	DepthMapNode* newNode = new DepthMapNode();
	newNode->Initialize(index);
	mNodes.push_back(newNode);
	return newNode;
}
//----------------------------------------------------------------------------------------------------

LightNode* SceneManager::CreateLightNode()
{
	LightNode* newNode = new LightNode();
	newNode->Initialize();
	mNodes.push_back(newNode);
	return newNode;
}

//----------------------------------------------------------------------------------------------------

MaterialNode* SceneManager::CreateMaterialNode()
{
	MaterialNode* newNode = new MaterialNode();
	newNode->Initialize();
	mNodes.push_back(newNode);
	return newNode;
}

//----------------------------------------------------------------------------------------------------

MeshNode* SceneManager::CreateMeshNode()
{
	MeshNode* newNode = new MeshNode();
	mNodes.push_back(newNode);
	return newNode;
}

//----------------------------------------------------------------------------------------------------

ModelNode* SceneManager::CreateModelNode(const char* fileName)
{
	ModelNode* newNode = new ModelNode();
	newNode->GetModel().LoadModelBinary(fileName);
	newNode->InitializeSubNodes(this);
	mNodes.push_back(newNode);
	return newNode;
}

//----------------------------------------------------------------------------------------------------

ModelNode* SceneManager::CreateModelNodeWithBones(const char* fileName)
{
	ModelNode* newNode = new ModelNode();
	newNode->GetModel().LoadModelBinaryWithBones(fileName);
	newNode->InitializeSubNodes(this);
	mNodes.push_back(newNode);
	return newNode;
}

//----------------------------------------------------------------------------------------------------

RasterizerStateNode* SceneManager::CreateRasterizerNode()
{
	RasterizerStateNode* newNode = new RasterizerStateNode();
	mNodes.push_back(newNode);
	return newNode;
}

//----------------------------------------------------------------------------------------------------

RenderTargetNode* SceneManager::CreateRenderTargetNode(uint32_t index)
{
	RenderTargetNode* newNode = new RenderTargetNode();
	newNode->Initialize(index);
	mNodes.push_back(newNode);
	return newNode;
}

//----------------------------------------------------------------------------------------------------

SamplerNode* SceneManager::CreateSamplerNode(Sampler::Filter filter, Sampler::AddressMode addressMode)
{
	SamplerNode* newNode = new SamplerNode();
	newNode->Initialize(filter, addressMode);
	mNodes.push_back(newNode);
	return newNode;
}

SamplerNode* SceneManager::CreateSamplerNode(Sampler::Filter filter, Sampler::AddressMode addressMode, uint32_t index)
{
	SamplerNode* newNode = new SamplerNode();
	newNode->Initialize(filter, addressMode, index);
	mNodes.push_back(newNode);
	return newNode;
}

//----------------------------------------------------------------------------------------------------

ShaderNode* SceneManager::CreateShaderNode()
{
	ShaderNode* newNode = new ShaderNode();
	mNodes.push_back(newNode);
	return newNode;
}

//----------------------------------------------------------------------------------------------------

TextureNode* SceneManager::CreateTextureNode(ShaderStage shaderStage)
{
	TextureNode* newNode = new TextureNode(shaderStage);
	mNodes.push_back(newNode);
	return newNode;
}

//----------------------------------------------------------------------------------------------------

TransformNode* SceneManager::CreateTransformNode()
{
	TransformNode* newNode = new TransformNode(mMatrixStack);
	newNode->Initialize();
	mNodes.push_back(newNode);
	return newNode;
}

//----------------------------------------------------------------------------------------------------
SceneNode& SceneManager::GetRoot()
{
	return mpRoot;
}

void SceneManager::Purge()
{
	const uint32_t numNodes = (uint32_t)mNodes.size();
	for (uint32_t i = 0; i < numNodes; ++i)
	{
		mNodes[i]->Terminate();
	}
	for (uint32_t i = 0; i < numNodes; ++i)
	{
		SafeDelete(mNodes[i]);
	}
	mNodes.clear();
}

//----------------------------------------------------------------------------------------------------

void SceneManager::Update(float deltaTime)
{
	
	mpRoot.Update(deltaTime);

}

//----------------------------------------------------------------------------------------------------

void SceneManager::Render()
{
	
	mpRoot.Render();

}