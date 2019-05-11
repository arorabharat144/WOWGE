#ifndef INCLUDED_GRAPHICS_DEPTHMAP_H
#define INCLUDED_GRAPHICS_DEPTHMAP_H

#include "Texture.h"
#include "Types.h"

namespace WOWGE
{
namespace Graphics
{

class DepthMap
{
public:
	DepthMap();
	~DepthMap();

	DepthMap(const DepthMap&) = delete;
	DepthMap& operator=(const DepthMap&) = delete;

	void Initialize(uint32_t width, uint32_t height);
	void Terminate();

	void BeginRender();
	void EndRender();

	void BindTexture(uint32_t index);
	void UnbindTexture(uint32_t index);
private:

	ID3D11ShaderResourceView* mShaderResourceView;
	ID3D11DepthStencilView* mDepthStencilView;
	D3D11_VIEWPORT mViewport;
};

} //namespace Graphics
} //namespace WOWGE

#endif // !INCLUDED_GRAPHICS_DEPTHMAP_H
