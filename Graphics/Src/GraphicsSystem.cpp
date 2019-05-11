#include "Precompiled.h"
#include "GraphicsSystem.h"

using namespace WOWGE;
using namespace WOWGE::Graphics;

namespace
{
	const D3D_DRIVER_TYPE kDriverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	const UINT kNumDriverTypes = ARRAYSIZE(kDriverTypes);

	const D3D_FEATURE_LEVEL kFeatureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	const UINT kNumFeatureLevels = ARRAYSIZE(kFeatureLevels);

	GraphicsSystem* sGraphicsSystem = nullptr;
}

void GraphicsSystem::StaticInitialize(HWND window, bool fullscreen)
{
	ASSERT(sGraphicsSystem == nullptr, "[Graphics::GraphicsSystem] System already initialized!");
	sGraphicsSystem = new GraphicsSystem();
	sGraphicsSystem->Initialize(window, fullscreen);
}

void GraphicsSystem::StaticTerminate()
{
	if (sGraphicsSystem != nullptr)
	{
		sGraphicsSystem->Terminate();
		SafeDelete(sGraphicsSystem);
	}
}

GraphicsSystem* GraphicsSystem::Get()
{
	ASSERT(sGraphicsSystem != nullptr, "[Graphics::GraphicsSystem] No system registered.");
	return sGraphicsSystem;
}

GraphicsSystem::GraphicsSystem()
	: mD3DDevice(nullptr)
	, mImmediateContext(nullptr)
	, mSwapChain(nullptr)
	, mRenderTargetView(nullptr)
	, mDepthStencilBuffer(nullptr)
	, mDepthStencilView(nullptr)
	, mDisableDepthStencil(nullptr)
	, mDriverType(D3D_DRIVER_TYPE_NULL)
	, mFeatureLevel(D3D_FEATURE_LEVEL_11_0)
{
}

GraphicsSystem::~GraphicsSystem()
{
	ASSERT(mD3DDevice == nullptr, "[Graphics::GraphicsSystem] Terminate() must be called to clean up!");
}

void GraphicsSystem::Initialize(HWND window, bool fullscreen)
{
	RECT rc = {};
	GetClientRect(window, &rc);
	UINT width = (UINT)(rc.right - rc.left);
	UINT height = (UINT)(rc.bottom - rc.top);

	UINT createDeviceFlags = 0;
	
	DXGI_SWAP_CHAIN_DESC descSwapChain = { 0 };
	descSwapChain.BufferCount = 1;
	descSwapChain.BufferDesc.Width = width;
	descSwapChain.BufferDesc.Height = height;
	descSwapChain.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	descSwapChain.BufferDesc.RefreshRate.Numerator = 60;
	descSwapChain.BufferDesc.RefreshRate.Denominator = 1;
	descSwapChain.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	descSwapChain.OutputWindow = window;
	descSwapChain.SampleDesc.Count = 1;
	descSwapChain.SampleDesc.Quality = 0;
	descSwapChain.Windowed = !fullscreen;

	HRESULT hr = S_OK;
	for (UINT driverTypeIndex = 0; driverTypeIndex < kNumDriverTypes; ++driverTypeIndex)
	{
		mDriverType = kDriverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain
		(
			nullptr,
			mDriverType,
			nullptr,
			createDeviceFlags,
			kFeatureLevels,
			kNumFeatureLevels,
			D3D11_SDK_VERSION,
			&descSwapChain,
			&mSwapChain,
			&mD3DDevice,
			&mFeatureLevel,
			&mImmediateContext
		);
		if (SUCCEEDED(hr))
		{
			break;
		}
	}
	ASSERT(SUCCEEDED(hr), "[Graphics::GraphicsSystem] Failed to create device or swap chain.");

	// Cache swap chain description
	mSwapChain->GetDesc(&mSwapChainDesc);

	// Create a render target view
	ID3D11Texture2D* backBuffer = nullptr;
	hr = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
	ASSERT(SUCCEEDED(hr), "[Graphics::GraphicsSystem] Failed to access swap chain buffer.");

	hr = mD3DDevice->CreateRenderTargetView(backBuffer, nullptr, &mRenderTargetView);
	SafeRelease(backBuffer);
	ASSERT(SUCCEEDED(hr), "[Graphics::GraphicsSystem] Failed to create render target view.");

	// Create depth stencil texture
	D3D11_TEXTURE2D_DESC descDepth = {};
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	hr = mD3DDevice->CreateTexture2D(&descDepth, nullptr, &mDepthStencilBuffer);
	ASSERT(SUCCEEDED(hr), "[Graphics::GraphicsSystem] Failed to create depth stencil buffer.");
	
	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	hr = mD3DDevice->CreateDepthStencilView(mDepthStencilBuffer, &descDSV, &mDepthStencilView);
	ASSERT(SUCCEEDED(hr), "[Graphics::GraphicsSystem] Failed to create depth stencil view.");
	
	// Set the render target view and depth stencil view
	mImmediateContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc.DepthEnable = false;
	mD3DDevice->CreateDepthStencilState(&depthStencilDesc, &mDisableDepthStencil);

	// Setup the viewport
	mViewport.Width = (FLOAT)width;
	mViewport.Height = (FLOAT)height;
	mViewport.MinDepth = 0.0f;
	mViewport.MaxDepth = 1.0f;
	mViewport.TopLeftX = 0;
	mViewport.TopLeftY = 0;
	mImmediateContext->RSSetViewports(1, &mViewport);
}

void GraphicsSystem::Terminate()
{
	SafeRelease(mDisableDepthStencil);
	SafeRelease(mDepthStencilView);
	SafeRelease(mDepthStencilBuffer);
	SafeRelease(mRenderTargetView);
	SafeRelease(mSwapChain);
	SafeRelease(mImmediateContext);
	SafeRelease(mD3DDevice);
}

void GraphicsSystem::BeginRender()
{
	const FLOAT clearColor[4] = { 0.5f, 0.5f, 0.5f, 0.5f }; //COLOR OF RENDER - BLACK(DEFAULT)
	mImmediateContext->ClearRenderTargetView(mRenderTargetView, clearColor);
	mImmediateContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void GraphicsSystem::EndRender()
{
	mSwapChain->Present(1, 0);
}

void GraphicsSystem::ToggleFullscreen()
{
	BOOL fullscreen;
	mSwapChain->GetFullscreenState(&fullscreen, nullptr);
	mSwapChain->SetFullscreenState(!fullscreen, nullptr);
}

void GraphicsSystem::ResetRenderTarget()
{
	ASSERT(mImmediateContext != nullptr, "[GraphicsSystem] Failed to reset render target.");
	mImmediateContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
}

void GraphicsSystem::ResetViewport()
{
	ASSERT(mImmediateContext != nullptr, "[GraphicsSystem] Failed to reset viewport.");
	mImmediateContext->RSSetViewports(1, &mViewport);
}

void GraphicsSystem::EnableDepthTesting(bool enable)
{
	ASSERT(mImmediateContext != nullptr, "[GraphicsSystem] Failed to set depth stencil state.");
	mImmediateContext->OMSetDepthStencilState(enable ? nullptr : mDisableDepthStencil, 0);
}

uint32_t GraphicsSystem::GetWidth() const
{
	ASSERT(mSwapChain != nullptr, "[GraphicsSystem] Failed to get swap chain buffer width.");
	return mSwapChainDesc.BufferDesc.Width;
}

uint32_t GraphicsSystem::GetHeight() const
{
	ASSERT(mSwapChain != nullptr, "[GraphicsSystem] Failed to get swap chain buffer width.");
	return mSwapChainDesc.BufferDesc.Height;
}

uint32_t GraphicsSystem::GetAspectRatio() const
{
	ASSERT(mSwapChain != nullptr, "[GraphicsSystem] Failed to get swap chain buffer aspect");
	return mSwapChainDesc.BufferDesc.Width / mSwapChainDesc.BufferDesc.Height;
}

void GraphicsSystem::ClearDepthBuffer()
{
	ASSERT(mImmediateContext != nullptr, "[GraphicsSystem] Failed to clear depth buffer.");
	mImmediateContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}