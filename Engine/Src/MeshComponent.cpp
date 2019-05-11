#include "Precompiled.h"
#include "MeshComponent.h"

#include "World.h"
#include "ResourceManager.h"

#include "PrimitiveMeshResource.h"
#include "TextureResource.h"
#include "TransformComponent.h"
#include "CameraComponent.h"
#include "VertexShaderResource.h"
#include "PixelShaderResource.h"

using namespace WOWGE;
using namespace WOWGE::Engine;

META_DERIVED_BEGIN(MeshComponent, Component)
	META_FIELD_BEGIN
	META_FIELD(mMeshType, "MeshType")
	META_FIELD(mTexture, "Texture")
	META_FIELD(mVertexShader, "VertexShader")
	META_FIELD(mPixelShader, "PixelShader")
	META_FIELD_END
META_CLASS_END;

MeshComponent::MeshComponent()
{

}

MeshComponent::~MeshComponent()
{

}

void MeshComponent::Initialize()
{
	mMeshBuffer = std::make_unique<Graphics::MeshBuffer>();
	mConstantBuffer = std::make_unique<TransformCB>();
	mSampler = std::make_unique<Graphics::Sampler>();

	PrimitiveMeshResource* meshResource{ dynamic_cast<PrimitiveMeshResource*>(GetOwner().GetWorld().GetResourceManager()->GetResource(mMeshType)) };
	if (meshResource)
	{
		mMeshBuffer->Initialize(*meshResource->GetMesh());
	}

	mConstantBuffer->Initialize();
	mSampler->Initialize(Graphics::Sampler::Filter::Anisotropic, Graphics::Sampler::AddressMode::Wrap);

	mTransformComponent = GetOwner().GetComponent<TransformComponent>();
}

void MeshComponent::Terminate()
{
	mMeshBuffer->Terminate();
	mConstantBuffer->Terminate();
	mSampler->Terminate();
}

void MeshComponent::Render()
{
	VertexShaderResource* vertexShaderResource{ dynamic_cast<VertexShaderResource*>(GetOwner().GetWorld().GetResourceManager()->GetResource(mVertexShader)) };
	if (vertexShaderResource)
	{
		vertexShaderResource->Render();
	}

	PixelShaderResource* pixelShaderResource{ dynamic_cast<PixelShaderResource*>(GetOwner().GetWorld().GetResourceManager()->GetResource(mPixelShader)) };
	if (pixelShaderResource)
	{
		pixelShaderResource->Render();
	}

	Maths::Matrix44 world
	{
		mTransformComponent->GetScale().x, 0.0f, 0.0f, 0.0f,
		0.0f, mTransformComponent->GetScale().y, 0.0f, 0.0f,
		0.0f, 0.0f, mTransformComponent->GetScale().z, 0.0f,
		mTransformComponent->GetPosition().x, mTransformComponent->GetPosition().y, mTransformComponent->GetPosition().z, 1.0f
	};

	TransformData data;
	data.world = Maths::Transpose(world);
	data.view = Maths::Transpose(GetOwner().GetWorld().Find("MainCamera").Get()->GetComponent<CameraComponent>()->GetViewMatrix());
	data.projection = Maths::Transpose(GetOwner().GetWorld().Find("MainCamera").Get()->GetComponent<CameraComponent>()->GetProjectionMatrix());
	mConstantBuffer->Set(data);

	mConstantBuffer->BindVS(0);
	mConstantBuffer->BindPS(0);

	TextureResource* textureResource{ dynamic_cast<TextureResource*>(GetOwner().GetWorld().GetResourceManager()->GetResource(mTexture)) };
	if (textureResource)
	{
		textureResource->Render();
	}

	mSampler->BindVS(0);
	mSampler->BindPS(0);

	mMeshBuffer->Render();
}