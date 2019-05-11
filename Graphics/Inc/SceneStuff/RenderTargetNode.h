#ifndef INCLUDED_GRAPHICS_RENDERTARGETNODE_H
#define INCLUDED_GRAPHICS_RENDERTARGETNODE_H

#include "RenderTarget.h"
#include "SceneNode.h"

namespace WOWGE
{
namespace Graphics
{

class RenderTargetNode : public SceneNode
{
public:
	RenderTargetNode();
	~RenderTargetNode();

	void Initialize(uint32_t index);
	void Terminate();

	void SetRenderTarget(RenderTarget* renderTarget);

	void SetHasTexture(bool status);
private:
	void OnPreRender() override;
	void OnRender() override;
	void OnPostRender() override;
private:
	RenderTarget* mRenderTarget;
	uint32_t mIndex;
	bool mHasTexture;
};

} //namespace Graphics
} //namespace WOWGE

#endif // !INCLUDED_GRAPHICS_RENDERTARGETNODE_H
