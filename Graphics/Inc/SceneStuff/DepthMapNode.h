#ifndef INCLUDED_GRAPHICS_DEPTHMAPNODE_H
#define INCLUDED_GRAPHICS_DEPTHMAPNODE_H

#include "DepthMap.h"
#include "SceneNode.h"

namespace WOWGE
{
namespace Graphics
{

class DepthMapNode : public SceneNode
{
public:
	DepthMapNode();
	~DepthMapNode();

	void Initialize(uint32_t index);
	void Terminate();

	void SetDepthMap(DepthMap* depthMap);

	void SetHasTexture(bool status);
private:
	void OnPreRender() override;
	void OnRender() override;
	void OnPostRender() override;
private:
	DepthMap* mDepthMap;
	uint32_t mIndex;
	bool mHasTexture;
};

} //namespace Graphics
} //namespace WOWGE

#endif // !INCLUDED_GRAPHICS_DEPTHMAPNODE_H
