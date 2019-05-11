#include "Precompiled.h"
#include "PixelShader.h"

#define D3D3_COMPILE_STANDARD_FILE_INCLUDE ((ID3DInclude*)(UINT_PTR)1)

using namespace WOWGE;
using namespace WOWGE::Graphics;

PixelShader::PixelShader():
	mPixelShader(nullptr)
{

}

PixelShader::~PixelShader()
{

}

void PixelShader::Initialize(const wchar_t * fileName)
{
	auto graphicsSystem = Graphics::GraphicsSystem::Get();

	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;

	///////////////////////////////////////
	// Compile and create the pixel shader

	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;

	HRESULT hr = D3DCompileFromFile
	(
		fileName,
		nullptr,
		D3D3_COMPILE_STANDARD_FILE_INCLUDE,
		"PS",
		"ps_5_0",
		shaderFlags,
		0,
		&shaderBlob,
		&errorBlob
	);

	ASSERT(SUCCEEDED(hr), "Failed to compile shader. Error: %s", (const char*)errorBlob->GetBufferPointer());
	SafeRelease(errorBlob);

	// Create pixel shader
	graphicsSystem->GetDevice()->CreatePixelShader
	(
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		nullptr,
		&mPixelShader
	);
	SafeRelease(shaderBlob);
}

void PixelShader::Initialize(const wchar_t* shader, const char* entryPoint, const char* shaderModel)
{
	auto graphicsSystem = Graphics::GraphicsSystem::Get();

	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;

	///////////////////////////////////////
	// Compile and create the pixel shader

	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;

	HRESULT hr = D3DCompileFromFile
	(
		shader,
		nullptr,
		D3D3_COMPILE_STANDARD_FILE_INCLUDE,
		entryPoint,
		shaderModel,
		shaderFlags,
		0,
		&shaderBlob,
		&errorBlob
	);

	ASSERT(SUCCEEDED(hr), "Failed to compile shader. Error: %s", (const char*)errorBlob->GetBufferPointer());
	SafeRelease(errorBlob);

	// Create pixel shader
	graphicsSystem->GetDevice()->CreatePixelShader
	(
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		nullptr,
		&mPixelShader
	);
	SafeRelease(shaderBlob);
}

void PixelShader::InitializeWithoutFile(const char* shader, const char* entryPoint, const char* shaderModel)
{
	auto graphicsSystem = Graphics::GraphicsSystem::Get();

	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;

	///////////////////////////////////////
	// Compile and create the pixel shader

	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;

	HRESULT hr = D3DCompile(shader, strlen(shader), nullptr, nullptr, nullptr, entryPoint, shaderModel, shaderFlags, 0, &shaderBlob, &errorBlob);

	ASSERT(SUCCEEDED(hr), "Failed to compile shader. Error: %s", (const char*)errorBlob->GetBufferPointer());
	SafeRelease(errorBlob);

	// Create pixel shader
	graphicsSystem->GetDevice()->CreatePixelShader
	(
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		nullptr,
		&mPixelShader
	);
	SafeRelease(shaderBlob);
}

void PixelShader::Terminate()
{
	SafeRelease(mPixelShader);
}

void PixelShader::Bind()
{
	ID3D11DeviceContext* context = Graphics::GraphicsSystem::Get()->GetContext();

	context->PSSetShader(mPixelShader, nullptr, 0);
}
