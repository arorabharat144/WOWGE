#ifndef INCLUDED_GRAPHICS_CAMERA_H
#define INCLUDED_GRAPHICS_CAMERA_H

namespace WOWGE
{
namespace Graphics
{

class Camera
{
public:
	Camera();
	~Camera();

	void Initialize(float fov, float windowWidth, float windowHeight, float nearPlane, float farPlane);

	Maths::Vector3 GetPosition() const;
	Maths::Vector3 GetDirection() const;
	Maths::Vector3 GetUp() const;
	//Maths::Vector3 GetRight() const;

	void SetPosition(const Maths::Vector3& position);
	void SetDirection(const Maths::Vector3& direction);
	void SetLookAt(const Maths::Vector3& target);

	float GetFOV() const;
	float GetNearPlane() const;
	float GetFarPlane() const;
	float GetAspectRatio() const;

	void SetFOV(float fov);
	void SetNearPlane(float nearPlane);
	void SetFarPlane(float farPlane);
	void SetAspectRatio(float aspectRatio);

	Maths::Matrix44 GetViewMatrix() const;
	Maths::Matrix44 GetProjectionMatrix(/*uint32_t width, uint32_t height*/) const;

	void Walk(float distance);
	void Strafe(float distance);
	void Rise(float distance);

	void Yaw(float degree); //Along x-axis
	void Pitch(float degree); //Along y-axis

	void UpdateViewMatrix();
	void InitializePerspectiveProjectionMatrix();
	void InitializeOrthographicProjectionMatrix(float width, float height);

	Maths::Ray ScreenPointToRay(int posX, int posY, uint32_t screenWidth, uint32_t screenHeight);
private:
	Maths::Vector3 GetRight();

private:
	Maths::Matrix44 mView; //View matrix
	Maths::Matrix44 mProjection; //Projection matrix

	Maths::Vector3 mPosition; //Position of the camera
	Maths::Vector3 mDirection; //Lookat Vector
	Maths::Vector3 mUp; //Up vector of the camera (Set up as 0,1,0 for this camera as we are assuming that yaxis is up always)

	float mFOV; //Field of view for the camera
	float mNearPlane;
	float mFarPlane;

	float mAspectRatio;
};

} //namespace Graphics
} //namespace WOWGE

#endif // !INCLUDED_GRAPHICS_CAMERA_H
