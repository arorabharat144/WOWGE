//====================================================================================================
// Filename:	SceneNode.cpp
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Graphics\Src\Precompiled.h"

#include "Graphics\Inc\SceneStuff\SceneNode.h"

using namespace WOWGE;
using namespace WOWGE::Graphics;
//====================================================================================================
// Class Definitions
//====================================================================================================

SceneNode::SceneNode()
	: mpParent(nullptr)
	, mName("Unknown")
	, mId(-1)
	, mEnabled(true)
{
	mId++;
}

//----------------------------------------------------------------------------------------------------

SceneNode::~SceneNode()
{
	// Empty
}

//----------------------------------------------------------------------------------------------------

bool SceneNode::AddChild(SceneNode* child)
{
	// Add child to list and set self to be parent
	mChildren.push_back(child);
	child->mpParent = this;
	return true;
}

//----------------------------------------------------------------------------------------------------

bool SceneNode::RemoveChild(const char* name)
{
	const uint32_t numChildren = (uint32_t)mChildren.size();
	for (uint32_t i = 0; i < numChildren; ++i)
	{
		if (mChildren[i]->mName == name)
		{
			mChildren[i] = mChildren[numChildren - 1];
			mChildren.pop_back();
			return true;
		}
	}
	return false;
}

//----------------------------------------------------------------------------------------------------

bool SceneNode::RemoveChild(uint32_t id)
{
	const uint32_t numChildren = (uint32_t)mChildren.size();
	for (uint32_t i = 0; i < numChildren; ++i)
	{
		if (mChildren[i]->mId == id)
		{
			mChildren[i] = mChildren[numChildren - 1];
			mChildren.pop_back();
			return true;
		}
	}
	return false;
}

//----------------------------------------------------------------------------------------------------

void SceneNode::Update(float deltaTime)
{
	if (true == mEnabled)
	{
		OnUpdate(deltaTime);
	}

	for (SceneNode* child : mChildren)
	{
		child->Update(deltaTime);
	}
}

//----------------------------------------------------------------------------------------------------

void SceneNode::Render()
{
	if (true == mEnabled)
	{
		OnPreRender();
		OnRender();
	}
	
	for (SceneNode* child : mChildren)
	{
		child->Render();
	}

	if (true == mEnabled)
	{
		OnPostRender();
	}
}