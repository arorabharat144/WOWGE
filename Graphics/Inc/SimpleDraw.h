#ifndef INCLUDED_GRAPHICS_SIMPLEDRAW_H
#define INCLUDED_GRAPHICS_SIMPLEDRAW_H

namespace WOWGE
{
namespace Graphics
{
class Camera;

namespace SimpleDraw 
{

enum class Axis
{
	XAxis,
	YAxis,
	ZAxis
};

// Functions to startup/shutdown simple draw
void Initialize(uint32_t maxVertices = 1000000);
void Terminate();

// Functions for world space rendering
void AddLine(const Maths::Vector3& v0, const Maths::Vector3& v1, const Maths::Vector4& color);
void AddLine(float x0, float y0, float z0, float x1, float y1, float z1, const Maths::Vector4& color);

void AddSquare(const Maths::Vector3& centre, float size, const Maths::Vector4& color, Axis axis);
void AddAABB(const Maths::AABB& aabb, const Maths::Vector4& color);

//void AddAABB(const Maths::Vector3& min, const Maths::Vector3& max, const Maths::Vector4& color);
//void AddAABB(const Maths::Vector3& center, float radius, const Maths::Vector4& color);
//void AddAABB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ, const Maths::Vector4& color);
//void AddOBB(const Maths::OBB& obb, const Maths::Vector4& color);

void AddPlane(const Maths::Plane& plane, const Maths::Vector3& referencePoint, float size, float spacing, const Maths::Vector4& color);

//void AddSphere(const Maths::Sphere& sphere, const Maths::Vector4& color, uint32_t slices = 8, uint32_t rings = 4);

void AddSphere(const Maths::Vector3& center, float radius, const Maths::Vector4& color, uint32_t slices = 8, uint32_t rings = 4);
void AddSphere(float x, float y, float z, float radius, const Maths::Vector4& color, uint32_t slices = 8, uint32_t rings = 4);
void AddTransform(const Maths::Matrix44& transform);

// Functions for screen space rendering
void AddScreenLine(const Maths::Vector2& v0, const Maths::Vector2& v1, const Maths::Vector4& color);
void AddScreenLine(float x0, float y0, float x1, float y1, const Maths::Vector4& color);
//void AddScreenRect(const Maths::Rect& rect, const Maths::Vector4& color);

void AddScreenRect(const Maths::Vector2& min, const Maths::Vector2& max, const Maths::Vector4& color);
void AddScreenRect(float left, float top, float right, float bottom, const Maths::Vector4& color);
void AddScreenTriangle(const Maths::Vector2& center, float size, float rotation, const Maths::Vector4& color);
void AddScreenCircle(const Maths::Circle& circle, const Maths::Vector4& color);
void AddScreenCircle(const Maths::Vector2& center, float r, const Maths::Vector4& color);
void AddScreenCircle(float x, float y, float r, const Maths::Vector4& color);

// Function to actually render all the geometry.
void Render(const Camera& camera);

} // namespace SimpleDraw
} // namespace Graphics
} // namespace Engine

#endif // #ifndef INCLUDED_GRAPHICS_SIMPLEDRAW_H