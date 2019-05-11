#ifndef INCLUDED_GRAPHICS_SCENENODE_H
#define INCLUDED_GRAPHICS_SCENENODE_H

//====================================================================================================
// Filename:	SceneNode.h
// Description:	Class for a 3D scene node.
//====================================================================================================

#include "Core\Inc\Common.h"

//====================================================================================================
// Class Declarations
//====================================================================================================

namespace WOWGE
{
namespace Graphics
{

class SceneNode
{
public:
	SceneNode();
	virtual ~SceneNode();

	bool AddChild(SceneNode* child);
	bool RemoveChild(const char* name);
	bool RemoveChild(uint32_t id);

	void Update(float deltaTime);
	void Render();
	virtual void Terminate() {}

	void SetName(const char* name) { mName = name; }
	void SetId(uint32_t id) { mId = id; }
	void SetEnabled(bool enabled) { mEnabled = enabled; }

	const char* GetName() const { return mName.c_str(); }
	uint32_t GetId() const { return mId; }
	bool GetEnabled() const { return mEnabled; }

protected:
	typedef std::vector<SceneNode*> SceneNodeList;

	virtual void OnUpdate(float deltaTime) {}
	virtual void OnPreRender() {}
	virtual void OnRender() {}
	virtual void OnPostRender() {}



	SceneNode* mpParent;
	SceneNodeList mChildren;

	std::string mName;
	uint32_t mId;

	bool mEnabled;
};

} //namespace Graphics
} //namespace WOWGE

#endif //INCLUDED_GRAPHICS_SCENENODE_H