#include "Precompiled.h"
#include "Texture.h"

#include "GraphicsSystem.h"
#include <External/DirectXTK/Inc/DDSTextureLoader.h>
#include <External/DirectXTK/Inc/WICTextureLoader.h>

using namespace WOWGE;
using namespace WOWGE::Graphics;

Texture::Texture()
	: mShaderResourceView(nullptr)
{
}

Texture::~Texture()
{
	ASSERT(mShaderResourceView == nullptr, "[Texture] Texture not released!");
}

void Texture::Initialize(const char* filename)
{
	ID3D11Device* device = GraphicsSystem::Get()->GetDevice();
	ID3D11DeviceContext* context = GraphicsSystem::Get()->GetContext();

	wchar_t wbuffer[1024];
	mbstowcs_s(nullptr, wbuffer, filename, 1024);

	if (strstr(filename, ".dds") != nullptr)
	{
		DirectX::CreateDDSTextureFromFile(device, context, wbuffer, nullptr, &mShaderResourceView);
	}
	else
	{
		DirectX::CreateWICTextureFromFile(device, context, wbuffer, nullptr, &mShaderResourceView);
	}
}

void Texture::Initialize(const void* data, uint32_t width, uint32_t height)
{
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory(&initData, sizeof(initData));
	initData.pSysMem = data;
	initData.SysMemPitch = width * 4;

	ID3D11Device* device = GraphicsSystem::Get()->GetDevice();

	ID3D11Texture2D* texture = nullptr;
	HRESULT hr = device->CreateTexture2D(&desc, &initData, &texture);
	ASSERT(SUCCEEDED(hr), "[Texture] Failed to create texture.");

	hr = device->CreateShaderResourceView(texture, nullptr, &mShaderResourceView);
	ASSERT(SUCCEEDED(hr), "[Texture] Failed to create shader resource view.");

	SafeRelease(texture);
}

void Texture::Terminate()
{
	SafeRelease(mShaderResourceView);
}

void Texture::BindVS(uint32_t slot) const
{
	GraphicsSystem::Get()->GetContext()->VSSetShaderResources(slot, 1, &mShaderResourceView);
}

void Texture::BindPS(uint32_t slot) const
{
	GraphicsSystem::Get()->GetContext()->PSSetShaderResources(slot, 1, &mShaderResourceView);
}

void Texture::UnbindVS(uint32_t slot) const
{
	static ID3D11ShaderResourceView* dummy = nullptr;
	GraphicsSystem::Get()->GetContext()->VSSetShaderResources(slot, 1, &dummy);
}

void Texture::UnbindPS(uint32_t slot) const
{
	static ID3D11ShaderResourceView* dummy = nullptr;
	GraphicsSystem::Get()->GetContext()->PSSetShaderResources(slot, 1, &dummy);
}