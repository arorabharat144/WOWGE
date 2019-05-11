#include "Precompiled.h"
#include "ConstantBuffer.h"

#include "GraphicsSystem.h"

using namespace WOWGE;
using namespace WOWGE::Graphics;

ConstantBuffer::ConstantBuffer()
	: mBuffer(nullptr)
{
}

ConstantBuffer::~ConstantBuffer()
{
	ASSERT(mBuffer == nullptr, "[ConstantBuffer] Buffer not released!");
}

void ConstantBuffer::Initialize(uint32_t bufferSize, const void* initData)
{
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof desc);
	desc.ByteWidth = bufferSize;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	D3D11_SUBRESOURCE_DATA subresourceData;
	D3D11_SUBRESOURCE_DATA* subresourceDataPtr = nullptr;
	if (initData != nullptr)
	{
		ZeroMemory(&subresourceData, sizeof(subresourceData));
		subresourceData.pSysMem = initData;
		subresourceDataPtr = &subresourceData;
	}

	GraphicsSystem::Get()->GetDevice()->CreateBuffer(&desc, subresourceDataPtr, &mBuffer);
}

void ConstantBuffer::Terminate()
{
	SafeRelease(mBuffer);
}

void ConstantBuffer::Set(const void* data) const
{
	GraphicsSystem::Get()->GetContext()->UpdateSubresource(mBuffer, 0, nullptr, data, 0, 0);
}

void ConstantBuffer::BindVS(uint32_t slot) const
{
	GraphicsSystem::Get()->GetContext()->VSSetConstantBuffers(slot, 1, &mBuffer);
}

void ConstantBuffer::BindGS(uint32_t slot) const
{
	GraphicsSystem::Get()->GetContext()->GSSetConstantBuffers(slot, 1, &mBuffer);
}

void ConstantBuffer::BindPS(uint32_t slot) const
{
	GraphicsSystem::Get()->GetContext()->PSSetConstantBuffers(slot, 1, &mBuffer);
}

void ConstantBuffer::BindCS(uint32_t slot) const
{
	GraphicsSystem::Get()->GetContext()->CSSetConstantBuffers(slot, 1, &mBuffer);
}