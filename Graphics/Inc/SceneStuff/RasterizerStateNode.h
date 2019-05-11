#ifndef INCLUDED_GRAPHICS_RASTERIZERSTATENODE_H
#define INCLUDED_GRAPHICS_RASTERIZERSTATENODE_H

#include "SceneNode.h"
#include "RasterizerState.h"

namespace WOWGE
{
namespace Graphics
{

class RasterizerStateNode : public SceneNode
{
public:
	RasterizerStateNode(std::string name = "RasterizerStateNode");
	~RasterizerStateNode();

	void SetRasterizerStateNode(RasterizerState* rs) { mRasterizerState = rs; }

private:
	void OnPreRender() override;
	void OnPostRender() override;

private:
	RasterizerState* mRasterizerState;
};

} //namespace Graphics
} //namespace WOWGE

#endif // !INCLUDED_GRAPHICS_RASTERIZERSTATENODE_H
