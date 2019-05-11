#include "Graphics\Src\Precompiled.h"

#include "SceneStuff\RasterizerStateNode.h"

using namespace WOWGE;
using namespace WOWGE::Graphics;

RasterizerStateNode::RasterizerStateNode(std::string name)
	: mRasterizerState(nullptr)
{
	mName = std::move(name);
}

RasterizerStateNode::~RasterizerStateNode()
{

}

void RasterizerStateNode::OnPreRender()
{
	mRasterizerState->Set();
}

void RasterizerStateNode::OnPostRender()
{
	mRasterizerState->Clear();
}