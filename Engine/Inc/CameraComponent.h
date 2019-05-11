#ifndef INCLUDED_ENGINE_CAMERACOMPONENT_H
#define INCLUDED_ENGINE_CAMERACOMPONENT_H

#include "Component.h"

namespace WOWGE
{
namespace Engine
{

class TransformComponent;

class CameraComponent : public Component
{
public:
	META_CLASS_DECLARE

	CameraComponent();
	~CameraComponent() override;

	void Initialize() override;
	void Terminate() override;

	void Update(float deltaTime) override;

	void Render() override;

	Maths::Matrix44 GetViewMatrix() const;
	Maths::Matrix44 GetProjectionMatrix() const;
private:
	const TransformComponent* mTransformComponent;

	float mFOV;
	float mNearPlane;
	float mFarPlane;

	float mSpeed{ 20.0f };
	float mBoost{ 5.0f };
	float mRotationSpeed{ 5.0f };

	float mOrthographicWidth;
	float mOrthographicHeight;

	std::string mMode;

	bool mIsMainCamera;

	std::unique_ptr<Graphics::Camera> mCamera;
};

} //namespace Engine
} //namespace WOWGE

#endif // !INCLUDED_ENGINE_CAMERACOMPONENT_H
