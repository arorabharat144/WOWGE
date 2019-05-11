#include "Precompiled.h"

#include "DepthMap.h"

#include "GraphicsSystem.h"

using namespace WOWGE;
using namespace WOWGE::Graphics;

DepthMap::DepthMap() :
	mShaderResourceView(nullptr),
	mDepthStencilView(nullptr)
{

}

DepthMap::~DepthMap()
{
	ASSERT(mShaderResourceView == nullptr, "[RenderTarget] ShaderResourceView not released");
	ASSERT(mDepthStencilView == nullptr, "[RenderTarget] DepthStencilView not released");
}

void DepthMap::Initialize(uint32_t width, uint32_t height)
{
	D3D11_TEXTURE2D_DESC desc = {};
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	ID3D11Device* device = GraphicsSystem::Get()->GetDevice();

	ID3D11Texture2D* texture = nullptr;
	HRESULT hr = device->CreateTexture2D(&desc, nullptr, &texture);
	ASSERT(SUCCEEDED(hr), "[RenderTarget] Failed to create render texture");

	D3D11_SHADER_RESOURCE_VIEW_DESC resourceDesc = {};
	resourceDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	resourceDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
	resourceDesc.Texture2D.MipLevels = 1;

	hr = device->CreateShaderResourceView(texture, &resourceDesc, &mShaderResourceView);
	ASSERT(SUCCEEDED(hr), "[RenderTarget] Failed to create shader resource view");

	D3D11_DEPTH_STENCIL_VIEW_DESC depthDesc = {};
	depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

	hr = device->CreateDepthStencilView(texture, &depthDesc, &mDepthStencilView);
	ASSERT(SUCCEEDED(hr), "[RenderTarget] Failed to create depth stencil view");
	SafeRelease(texture);

	mViewport.TopLeftX = 0.0f;
	mViewport.TopLeftY = 0.0f;
	mViewport.Width = (float)width;
	mViewport.Height = (float)height;
	mViewport.MinDepth = 0.0f;
	mViewport.MaxDepth = 1.0f;
}

void DepthMap::Terminate()
{
	SafeRelease(mShaderResourceView);
	SafeRelease(mDepthStencilView);
}

void DepthMap::BeginRender()
{
	ID3D11DeviceContext* context = GraphicsSystem::Get()->GetContext();
	context->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_DEPTH, 1.0f, 0);

	static ID3D11RenderTargetView* nullRenderTargetView = nullptr;
	context->OMSetRenderTargets(1, &nullRenderTargetView, mDepthStencilView);

	context->RSSetViewports(1, &mViewport);
}

void DepthMap::EndRender()
{
	GraphicsSystem::Get()->ResetRenderTarget();
	GraphicsSystem::Get()->ResetViewport();
}

void DepthMap::BindTexture(uint32_t index)
{
	GraphicsSystem::Get()->GetContext()->PSSetShaderResources(index, 1, &mShaderResourceView);
}

void DepthMap::UnbindTexture(uint32_t index)
{
	static ID3D11ShaderResourceView* dummy = nullptr;
	GraphicsSystem::Get()->GetContext()->PSSetShaderResources(index, 1, &dummy);
}