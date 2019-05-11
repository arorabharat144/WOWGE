#include "Graphics\Src\Precompiled.h"

#include "Graphics\Inc\SceneStuff\DepthMapNode.h"

using namespace WOWGE;
using namespace WOWGE::Graphics;

DepthMapNode::DepthMapNode() :
	mHasTexture(false)
{

}

DepthMapNode::~DepthMapNode()
{

}

void DepthMapNode::Initialize(uint32_t index)
{
	mIndex = index;
}

void DepthMapNode::Terminate()
{
	mDepthMap->Terminate();
}

void DepthMapNode::SetDepthMap(DepthMap* depthMap)
{
	mDepthMap = depthMap;
}

void DepthMapNode::SetHasTexture(bool status)
{
	mHasTexture = status;
}

void DepthMapNode::OnPreRender()
{
	if (mHasTexture == false)
	{
		mDepthMap->BeginRender();
	}
	else
	{
		mDepthMap->BindTexture(mIndex);
	}
}

void DepthMapNode::OnRender()
{

}

void DepthMapNode::OnPostRender()
{
	if (mHasTexture == false)
	{
		mDepthMap->EndRender();
	}
	else
	{
		mDepthMap->UnbindTexture(mIndex);
	}
}