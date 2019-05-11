#ifndef INCLUDED_GRAPHICS_SCENEMANAGER_H
#define INCLUDED_GRAPHICS_SCENEMANAGER_H

//====================================================================================================
// Filename:	SceneManager.h
// Description:	Class for a scene manager.
//====================================================================================================

#include "Graphics\Inc\SceneStuff\CameraNode.h"
#include "Graphics\Inc\SceneStuff\ConstantBufferNode.h"
#include "Graphics\Inc\SceneStuff\DepthMapNode.h"
#include "Graphics\Inc\SceneStuff\LightNode.h"
#include "Graphics\Inc\SceneStuff\MaterialNode.h"
#include "Graphics\Inc\SceneStuff\MeshNode.h"
#include "Graphics\Inc\SceneStuff\ModelNode.h"
#include "Graphics\Inc\SceneStuff\RasterizerStateNode.h"
#include "Graphics\Inc\SceneStuff\RenderTargetNode.h"
#include "Graphics\Inc\SceneStuff\SamplerNode.h"
#include "Graphics\Inc\SceneStuff\SceneNode.h"
#include "Graphics\Inc\SceneStuff\ShaderNode.h"
#include "Graphics\Inc\SceneStuff\TextureNode.h"
#include "Graphics\Inc\SceneStuff\TransformNode.h"

//====================================================================================================
// Class Declarations
//====================================================================================================

namespace WOWGE
{
namespace Graphics
{

class SceneManager
{
public:
	SceneManager(MatrixStack& ms);
	~SceneManager();

	CameraNode* CreateCameraNode();
	CameraNode* CreateCameraNode(uint32_t index);

	template<class T>
	ConstantBufferNode<T>* CreateConstantBufferNode(uint32_t index);
	DepthMapNode* CreateDepthMapNode(uint32_t index);
	LightNode* CreateLightNode();
	MaterialNode* CreateMaterialNode();
	MeshNode* CreateMeshNode();
	ModelNode* CreateModelNode(const char* fileName);
	ModelNode* CreateModelNodeWithBones(const char* fileName);

	RasterizerStateNode* CreateRasterizerNode();
	RenderTargetNode* CreateRenderTargetNode(uint32_t index);
	SamplerNode* CreateSamplerNode(Sampler::Filter filter, Sampler::AddressMode addressMode);
	SamplerNode* CreateSamplerNode(Sampler::Filter filter, Sampler::AddressMode addressMode, uint32_t index);

	ShaderNode* CreateShaderNode();
	TextureNode* CreateTextureNode(ShaderStage shaderStage);
	TransformNode* CreateTransformNode();

	SceneNode& GetRoot();

	void Purge();

	void Update(float deltaTime);
	void Render();

private:
	std::vector<SceneNode*> mNodes;

	MatrixStack& mMatrixStack;

	SceneNode mpRoot;
};

//----------------------------------------------------------------------------------------------------
template<class T>
ConstantBufferNode<T>* SceneManager::CreateConstantBufferNode(uint32_t index)
{
	ConstantBufferNode<T>* newNode = new ConstantBufferNode<T>();
	newNode->Initialize(index);
	mNodes.push_back(newNode);
	return newNode;
}

} //namespace Graphics
} //namespace WOWGE

#endif //INCLUDED_GRAPHICS_SCENEMANAGER_H