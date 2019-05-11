#ifndef INCLUDED_GRAPHICS_TRANSFORM_H
#define INCLUDED_GRAPHICS_TRANSFORM_H

#include "Math\Inc\Matrix44.h"
#include "Math\Inc\Vector3.h"

namespace WOWGE
{
namespace Graphics
{

class Transform
{
public:
	Transform();
	~Transform();

	Maths::Matrix44 GetTransformMatrix() const;
	void SetTransformMatrix(const Maths::Matrix44& transform);

	Maths::Vector3 GetPosition() const;
	void SetPosition(Maths::Vector3 position);

	Maths::Quaternion GetRotation() const;
	Maths::Vector3 GetEulerRotation() const;
	void SetRotation(Maths::Vector3 rotationInDegrees);
	void SetRotation(Maths::Quaternion rotation);

	Maths::Vector3 GetScale() const;
	void SetScale(Maths::Vector3 scale);

	//Maths::Vector3 GetPivot() const;
	//void SetPivot(const Maths::Vector3 pivot);
private:
	Maths::Vector3 mPosition;
	Maths::Quaternion mRotation;
	Maths::Vector3 mScale;
	//Maths::Vector3 mPivot;
};

} //namespace Graphics
} //namespace WOWGE

#endif // !INCLUDED_GRAPHICS_TRANSFORM_H
