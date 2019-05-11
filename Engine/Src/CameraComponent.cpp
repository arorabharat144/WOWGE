#include "Precompiled.h"
#include "CameraComponent.h"

#include "World.h"
#include "TransformComponent.h"

using namespace WOWGE;
using namespace WOWGE::Engine;

META_DERIVED_BEGIN(CameraComponent, Component)
	META_FIELD_BEGIN
	META_FIELD(mFOV, "FOV")
	META_FIELD(mNearPlane, "NearPlane")
	META_FIELD(mFarPlane, "FarPlane")
	META_FIELD(mSpeed, "Speed")
	META_FIELD(mBoost, "Boost")
	META_FIELD(mRotationSpeed, "RotationSpeed")
	META_FIELD(mOrthographicWidth, "OrthographicWidth")
	META_FIELD(mOrthographicHeight, "OrthographicHeight")
	META_FIELD(mFarPlane, "FarPlane")
	META_FIELD(mMode, "Mode")
	META_FIELD(mIsMainCamera, "IsMainCamera")
	META_FIELD_END
META_CLASS_END;

CameraComponent::CameraComponent()
{

}

CameraComponent::~CameraComponent()
{

}

void CameraComponent::Initialize()
{
	mCamera = std::make_unique<Graphics::Camera>();

	mCamera->Initialize(mFOV, static_cast<float>(Graphics::GraphicsSystem::Get()->GetWidth()), static_cast<float>(Graphics::GraphicsSystem::Get()->GetHeight()), mNearPlane, mFarPlane);

	if (mMode.compare("Perspective") == 0)
	{
		mCamera->InitializePerspectiveProjectionMatrix();
	}
	else if (mMode.compare("Orthographic") == 0)
	{
		mCamera->InitializeOrthographicProjectionMatrix(mOrthographicWidth, mOrthographicHeight);
	}

	mTransformComponent = GetOwner().GetComponent<TransformComponent>();
	mCamera->SetPosition(mTransformComponent->GetPosition());
}

void CameraComponent::Terminate()
{
	
}

void CameraComponent::Update(float deltaTime)
{
	if (mIsMainCamera)
	{
		Input::InputSystem* inputSystem{ Input::InputSystem::Get() };
		if (inputSystem->IsMouseDown(Input::MouseButton::RBUTTON))
		{
			float boost = inputSystem->IsKeyDown(Input::KeyCode::LSHIFT) ? mBoost : 1.0f;
			if (inputSystem->IsKeyDown(Input::KeyCode::W))
			{
				mCamera->Walk(mSpeed * boost * deltaTime);
			}
			if (inputSystem->IsKeyDown(Input::KeyCode::S))
			{
				mCamera->Walk(-mSpeed * boost * deltaTime);
			}
			if (inputSystem->IsKeyDown(Input::KeyCode::A))
			{
				mCamera->Strafe(-mSpeed * boost * deltaTime);
			}
			if (inputSystem->IsKeyDown(Input::KeyCode::D))
			{
				mCamera->Strafe(mSpeed * boost * deltaTime);
			}
			if (inputSystem->IsKeyDown(Input::KeyCode::Q))
			{
				mCamera->Rise(-mSpeed * boost * deltaTime);
			}
			if (inputSystem->IsKeyDown(Input::KeyCode::E))
			{
				mCamera->Rise(mSpeed * boost * deltaTime);
			}

			mCamera->Pitch(mRotationSpeed * deltaTime * inputSystem->GetMouseMoveY()); //MOUSE MOVEMENT
			mCamera->Yaw(mRotationSpeed * deltaTime * inputSystem->GetMouseMoveX()); //MOUSE MOVEMENT
		}
	}

	mCamera->UpdateViewMatrix();
}

void CameraComponent::Render()
{
	//Graphics::SimpleDraw::AddAABB({ mCamera->GetPosition(), {10.0f, 10.0f, 10.0f} }, Maths::Vector4::Red());
}

Maths::Matrix44 CameraComponent::GetViewMatrix() const
{
	return mCamera->GetViewMatrix();
}

Maths::Matrix44 CameraComponent::GetProjectionMatrix() const
{
	return mCamera->GetProjectionMatrix();
}