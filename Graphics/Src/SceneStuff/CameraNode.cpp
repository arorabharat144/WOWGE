//====================================================================================================
// Filename:	CameraNode.cpp
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Graphics\Src\Precompiled.h"

#include "Graphics\Inc\SceneStuff\CameraNode.h"

using namespace WOWGE;
using namespace WOWGE::Graphics;
//====================================================================================================
// Class Definitions
//====================================================================================================

CameraNode::CameraNode():
	mBindIndex(0)
{
	// Empty
}

//----------------------------------------------------------------------------------------------------

CameraNode::~CameraNode()
{
	Terminate();
}

//----------------------------------------------------------------------------------------------------
void CameraNode::Initialize(uint32_t bindindex)
{
	mBindIndex = bindindex;
	mConstantBuffer.Initialize();
}
//----------------------------------------------------------------------------------------------------

void CameraNode::Terminate()
{
	mConstantBuffer.Terminate();
}

//----------------------------------------------------------------------------------------------------

void CameraNode::OnUpdate(float deltaTime)
{
}

//----------------------------------------------------------------------------------------------------

void CameraNode::OnRender()
{
	ConstantBuffer constants;

	constants.matView[0] = Maths::Transpose(mCameras[0]->GetViewMatrix());
	constants.matProjection[0] = Maths::Transpose(mCameras[0]->GetProjectionMatrix());
	constants.viewPosition[0] = Maths::Vector4(mCameras[0]->GetPosition().x, mCameras[0]->GetPosition().y, mCameras[0]->GetPosition().z, 1.0f);

	constants.matView[1] = Maths::Transpose(mCameras[1]->GetViewMatrix());
	constants.matProjection[1] = Maths::Transpose(mCameras[1]->GetProjectionMatrix());
	constants.viewPosition[1] = Maths::Vector4(mCameras[1]->GetPosition().x, mCameras[1]->GetPosition().y, mCameras[1]->GetPosition().z, 1.0f);

	mConstantBuffer.Set(constants);
	
	mConstantBuffer.BindVS(mBindIndex);
	mConstantBuffer.BindPS(mBindIndex);
}