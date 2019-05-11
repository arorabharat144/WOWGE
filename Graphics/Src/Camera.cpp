#include "Precompiled.h"
#include "Camera.h"

using namespace WOWGE;
using namespace WOWGE::Graphics;

Camera::Camera():
	mPosition(0.0f, 0.0f, 0.0f),
	mDirection(0.0f, 0.0f, 1.0f),
	mUp(0.0f, 1.0f, 0.0f),
	//mRight(),
	mFOV(0.0f),
	mNearPlane(0.0f),
	mFarPlane(0.0f),
	mView(),
	mProjection()
{
	
}

Camera::~Camera()
{

}

void Camera::Initialize(float fov, float windowWidth, float windowHeight, float nearPlane, float farPlane)
{
	mFOV = fov;
	mAspectRatio = windowWidth / windowHeight;
	mNearPlane = nearPlane;
	mFarPlane = farPlane;
}

Maths::Vector3 Camera::GetPosition() const
{
	return mPosition;
}

Maths::Vector3 Camera::GetDirection() const
{
	return mDirection;
}

void Camera::SetPosition(const Maths::Vector3& position)
{
	mPosition = position;
}

void Camera::SetDirection(const Maths::Vector3& direction)
{
	mDirection = direction;
}

Maths::Vector3 Camera::GetUp() const
{
	return mUp;
}

//Maths::Vector3 Camera::GetRight() const
//{
//	return mRight;
//}

void Camera::SetLookAt(const Maths::Vector3& target)
{
	mDirection = Maths::Normalize(target - mPosition);
}

float Camera::GetFOV() const
{
	return mFOV;
}

float Camera::GetNearPlane() const
{
	return mNearPlane;
}

float Camera::GetFarPlane() const
{
	return mFarPlane;
}

float Camera::GetAspectRatio() const
{
	return mAspectRatio;
}

void Camera::SetFOV(float fov)
{
	mFOV = fov;
}

void Camera::SetNearPlane(float nearPlane)
{
	mNearPlane = nearPlane;
}

void Camera::SetFarPlane(float farPlane)
{
	mFarPlane = farPlane;
}

void Camera::SetAspectRatio(float aspectRatio)
{
	mAspectRatio = aspectRatio;
}

Maths::Matrix44 Camera::GetViewMatrix() const
{
	return mView; 
}

Maths::Matrix44 Camera::GetProjectionMatrix(/*uint32_t width, uint32_t height*/) const
{
	return mProjection; 
}

void Camera::Walk(float distance)
{
	mPosition += mDirection * distance;
}

void Camera::Strafe(float distance)
{
	mPosition += GetRight() * distance;
}

void Camera::Rise(float distance)
{
	mPosition += mUp * distance;
}

void Camera::Yaw(float degree)
{
	float radians = Maths::ConvertDegreeToRadian(degree);

	Maths::Matrix44 rotationMatrix = Maths::Matrix44::RotationY(radians);
	mUp = Maths::TransformNormal(mUp, rotationMatrix);
	mDirection = Maths::TransformNormal(mDirection, rotationMatrix);
	//Might have to renormalise 
}

void Camera::Pitch(float degree)
{
	float radians = Maths::ConvertDegreeToRadian(degree);

	Maths::Matrix44 rotationMatrix = Maths::MatrixRotationAxis(GetRight(), radians);
	mUp = Maths::TransformNormal(mUp, rotationMatrix);
	mDirection = Maths::TransformNormal(mDirection, rotationMatrix);
	//Might have to renormalise 
}

void Camera::UpdateViewMatrix()
{
	Maths::Vector3 right = GetRight();
	//Use copies of the original member variables

	mDirection = Maths::Normalize(mDirection);
	mUp = Maths::Normalize(Maths::Cross(mDirection, right));

	//Our up has changed. Calculate a right vector again
	right = Maths::Cross(mUp, mDirection);

	float x = -Maths::Dot(mPosition, right);
	float y = -Maths::Dot(mPosition, mUp);
	float z = -Maths::Dot(mPosition, mDirection);

	mView =
	{
		right.x, mUp.x, mDirection.x, 0.0f,
		right.y, mUp.y, mDirection.y, 0.0f,
		right.z, mUp.z, mDirection.z, 0.0f,
		x,       y,     z,            1.0f
	};
}

void Camera::InitializePerspectiveProjectionMatrix()
{
	float height = 1.0f / tanf(mFOV*0.5f); //CHANGE IT BACK TO MFOV
	float width = height / mAspectRatio;
	float q = mFarPlane / (mFarPlane - mNearPlane);

	mProjection =
	{
		width, 0.0f, 0.0f, 0.0f,
		0.0f, height, 0.0f, 0.0f,
		0.0f, 0.0f, q, 1.0f,
		0.0f, 0.0f, -q * mNearPlane, 0.0f
	};
}

void Camera::InitializeOrthographicProjectionMatrix(float width, float height)
{
	mProjection =
	{
		2.0f / width, 0.0f, 0.0f, 0.0f,
		0.0f, 2.0f / height, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f / (mFarPlane - mNearPlane), 0.0f,
		0.0f, 0.0f, -mNearPlane / (mFarPlane - mNearPlane), 1.0f
	};
}

Maths::Ray Camera::ScreenPointToRay(int posX, int posY, uint32_t screenWidth, uint32_t screenHeight)
{
	float halfWidth = screenWidth * 0.5f;
	float halfHeight = screenHeight * 0.5f;
	float tanFOV = tan(mFOV * 0.5f);

	float dx = tanFOV * (static_cast<float>(posX) / halfWidth - 1.0f) * mAspectRatio;
	float dy = tanFOV * (1.0f - static_cast<float>(posY) / halfHeight);

	Maths::Ray ray{ Maths::Vector3{}, Maths::Normalize({dx, dy, 1.0f}) };

	Maths::Matrix44 inverseViewMatrix = Maths::Inverse(mView);
	ray.origin = Maths::TransformCoord(ray.origin, inverseViewMatrix);
	ray.direction = Maths::TransformNormal(ray.direction, inverseViewMatrix);

	return ray;
}

Maths::Vector3 Camera::GetRight()
{
	return Maths::Normalize(Maths::Cross(mUp, mDirection));
}