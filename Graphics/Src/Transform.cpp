#include "Precompiled.h"

#include "Transform.h"

using namespace WOWGE;
using namespace WOWGE::Graphics;

Transform::Transform():
	mScale({1.0f, 1.0f, 1.0f})
{
}

Transform::~Transform()
{

}

Maths::Matrix44 Transform::GetTransformMatrix() const
{
	//Maths::Matrix44 pivot = Maths::Matrix44::Translation(mPivot.x, mPivot.y, mPivot.z);
	//Maths::Matrix44 negativePivot = Maths::Matrix44::Translation(-mPivot.x, -mPivot.y, -mPivot.z);

	return
	{
		Maths::Matrix44::Scaling(mScale.x, mScale.y, mScale.z)
		//*
		//pivot
		*
		Maths::QuaternionToMatrix(mRotation)
		//*
		//negativePivot
		*
		Maths::Matrix44::Translation(mPosition.x, mPosition.y, mPosition.z)
	};
}

void Transform::SetTransformMatrix(const Maths::Matrix44& transform)
{
	mPosition = { transform.m41, transform.m42, transform.m43 };
	mScale = { Maths::Magnitude({ transform.m11, transform.m12, transform.m13 }), Maths::Magnitude({ transform.m21,transform.m22,transform.m23 }), Maths::Magnitude({ transform.m31, transform.m32, transform.m33 }) };
	Maths::Matrix44 matrixWithoutScale{ Maths::Inverse(Maths::Matrix44::Scaling(mScale.x, mScale.y, mScale.z)) * transform };
	mRotation = Maths::MatrixToQuaternion(matrixWithoutScale);
	//mScale = { Maths::Magnitude({transform.m11, transform.m12, transform.m13}), Maths::Magnitude({transform.m21,transform.m22,transform.m23}), Maths::Magnitude({transform.m31, transform.m32, transform.m33}) };
}

Maths::Vector3 Transform::GetPosition() const
{
	return mPosition;
}

void Transform::SetPosition(Maths::Vector3 position)
{
	mPosition = position;
}

Maths::Quaternion Transform::GetRotation() const
{
	return mRotation;
}

Maths::Vector3 Transform::GetEulerRotation() const
{
	return Maths::ToEulerAngles(mRotation);
}

void Transform::SetRotation(Maths::Vector3 rotationInDegrees)
{
	float xRad = Maths::ConvertDegreeToRadian(rotationInDegrees.x);
	float yRad = Maths::ConvertDegreeToRadian(rotationInDegrees.y);
	float zRad = Maths::ConvertDegreeToRadian(rotationInDegrees.z);

	mRotation = Maths::ToQuaternion({ xRad, yRad, zRad });
}

void Transform::SetRotation(Maths::Quaternion rotation)
{
	mRotation = rotation;
}

Maths::Vector3 Transform::GetScale() const
{
	return mScale;
}

void Transform::SetScale(Maths::Vector3 scale)
{
	mScale = scale;
}

//Maths::Vector3 Transform::GetPivot() const
//{
//	return mPivot;
//}
//
//void Transform::SetPivot(const Maths::Vector3 pivot)
//{
//	mPivot = pivot;
//}