#include "Graphics\Src\Precompiled.h"
#include "Graphics\Inc\SceneStuff\RenderTargetNode.h"

using namespace WOWGE;
using namespace WOWGE::Graphics;

RenderTargetNode::RenderTargetNode():
	mHasTexture(false)
{

}

RenderTargetNode::~RenderTargetNode()
{

}

void RenderTargetNode::Initialize(uint32_t index)
{
	mIndex = index;
}

void RenderTargetNode::Terminate()
{
	mRenderTarget->Terminate();
}

void RenderTargetNode::SetRenderTarget(RenderTarget* renderTarget)
{
	mRenderTarget = renderTarget;
}

void RenderTargetNode::SetHasTexture(bool status)
{
	mHasTexture = status;
}

void RenderTargetNode::OnPreRender()
{
	if(mHasTexture == false)
	{
		mRenderTarget->BeginRender();
	}
	else
	{
		mRenderTarget->BindPS(mIndex);
	}
}

void RenderTargetNode::OnRender()
{
	
}

void RenderTargetNode::OnPostRender()
{
	if(mHasTexture == false)
	{
		mRenderTarget->EndRender();
	}
	else
	{
		mRenderTarget->UnbindPS(mIndex);
	}
}

