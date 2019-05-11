#ifndef INCLUDED_GRAPHICS_RENDERTARGET_H
#define INCLUDED_GRAPHICS_RENDERTARGET_H

#include "Texture.h"
#include "Types.h"

namespace WOWGE
{
namespace Graphics
{

class RenderTarget
{
public:
	RenderTarget();
	~RenderTarget();

	RenderTarget(const RenderTarget&) = delete;
	RenderTarget& operator=(const RenderTarget&) = delete;

	void Initialize(uint32_t width, uint32_t height, ColorFormat format);
	void Terminate();

	void BeginRender();
	void EndRender();

	void BindPS(uint32_t index);
	void UnbindPS(uint32_t index);

	void Copy(ID3D11Texture2D* texture);
private:

	ID3D11ShaderResourceView* mShaderResourceView;
	ID3D11RenderTargetView* mRenderTargetView;
	ID3D11DepthStencilView* mDepthStencilView;
	D3D11_VIEWPORT mViewport;
	D3D11_MAPPED_SUBRESOURCE mSubResource;
};

} //namespace Graphics
} //namespace WOWGE

#endif // !INCLUDED_GRAPHICS_RENDERTARGET_H
