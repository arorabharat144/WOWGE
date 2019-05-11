#include "Precompiled.h"
#include "VertexShader.h"
#include "VertexTypes.h"

#define D3D3_COMPILE_STANDARD_FILE_INCLUDE ((ID3DInclude*)(UINT_PTR)1)

using namespace WOWGE;
using namespace WOWGE::Graphics;


namespace
{
	std::vector<D3D11_INPUT_ELEMENT_DESC> GetVertexDescription(uint32_t vertexFormat)
	{
		std::vector<D3D11_INPUT_ELEMENT_DESC> desc;

		if (vertexFormat & VF_POSITION)
		{
			desc.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		}

		if (vertexFormat & VF_NORMAL)
		{
			desc.push_back({ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		}

		if (vertexFormat & VF_TANGENT)
		{
			desc.push_back({ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		}

		if (vertexFormat & VF_COLOR)
		{
			desc.push_back({ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		}

		if (vertexFormat & VF_UV)
		{
			desc.push_back({ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		}

		if (vertexFormat & VF_BINDEX)
		{
			desc.push_back({ "BLENDINDICES", 0, DXGI_FORMAT_R32G32B32A32_SINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		}

		if (vertexFormat & VF_BWEIGHT)
		{
			desc.push_back({ "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		}

		return desc;
	}
}

VertexShader::VertexShader():
	mVertexShader(nullptr),
	mInputLayout(nullptr)
{

}

VertexShader::~VertexShader()
{

}

void VertexShader::Initialize(const wchar_t* fileName, const char* entryPoint, const char* shaderModel, uint32_t vertexFormat)
{
	auto graphicsSystem = Graphics::GraphicsSystem::Get();

	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;

	///////////////////////////////////////
	// Compile and create the vertex shader
	// D3DCOMPILE_DEBUG flag improves the shader debugging experience

	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;

	//
	HRESULT hr = D3DCompileFromFile
	(
		fileName,
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		entryPoint,
		shaderModel,
		shaderFlags,
		0,
		&shaderBlob,
		&errorBlob
	);

	ASSERT(SUCCEEDED(hr), "Failed to compile shader. Error: %s", (const char*)errorBlob->GetBufferPointer());
	SafeRelease(errorBlob);

	// Create vertex shader
	graphicsSystem->GetDevice()->CreateVertexShader
	(
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		nullptr,
		&mVertexShader
	);

	const auto vertexDesc = GetVertexDescription(vertexFormat);

	// Create the input layout
	graphicsSystem->GetDevice()->CreateInputLayout
	(
		vertexDesc.data(),
		(UINT)vertexDesc.size(),
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		&mInputLayout
	);
	SafeRelease(shaderBlob);
}

void VertexShader::InitializeWithoutFile(const char* shader, const char* entryPoint, const char* shaderModel, uint32_t vertexFormat)
{
	auto graphicsSystem = Graphics::GraphicsSystem::Get();

	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;

	///////////////////////////////////////
	// Compile and create the vertex shader
	// D3DCOMPILE_DEBUG flag improves the shader debugging experience

	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;

	HRESULT hr = D3DCompile(shader, strlen(shader), nullptr, nullptr, nullptr, entryPoint, shaderModel, shaderFlags, 0, &shaderBlob, &errorBlob);

	ASSERT(SUCCEEDED(hr), "Failed to compile shader. Error: %s", (const char*)errorBlob->GetBufferPointer());
	SafeRelease(errorBlob);

	// Create vertex shader
	graphicsSystem->GetDevice()->CreateVertexShader
	(
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		nullptr,
		&mVertexShader
	);

	const auto vertexDesc = GetVertexDescription(vertexFormat);

	// Create the input layout
	graphicsSystem->GetDevice()->CreateInputLayout
	(
		vertexDesc.data(),
		(UINT)vertexDesc.size(),
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		&mInputLayout
	);
	SafeRelease(shaderBlob);
}

void VertexShader::Terminate()
{
	SafeRelease(mInputLayout);
	SafeRelease(mVertexShader);
}

void VertexShader::Bind()
{
	ID3D11DeviceContext* context = Graphics::GraphicsSystem::Get()->GetContext();

	context->IASetInputLayout(mInputLayout);
	context->VSSetShader(mVertexShader, nullptr, 0);
}