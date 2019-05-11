#include "Precompiled.h"

#include "Maths.h"

#include "OBB.h"

using namespace WOWGE;
using namespace WOWGE::Maths;

bool WOWGE::Maths::Intersect(const Vector3& point, const OBB& obb)
{
	// Compute the local-to-world/world-to-local matrices
	Matrix44 matTrans = Matrix44::Translation(obb.center.x, obb.center.y, obb.center.z);
	Matrix44 matRot = QuaternionToMatrix(obb.orientation);
	Matrix44 matWorld = matRot * matTrans;
	Matrix44 matWorldInv = Inverse(matWorld);

	// Transform the point into the OBB's local space
	Vector3 localPoint = TransformCoord(point, matWorldInv);
	AABB aabb({ 0.0f, 0.0f, 0.0f }, obb.extend);

	// Test against local AABB
	return Maths::Intersect(localPoint, aabb);
}