#ifndef INCLUDED_GRAPHICS_VERTEXTYPES_H
#define INCLUDED_GRAPHICS_VERTEXTYPES_H

#define VF_POSITION 0x1 << 0
#define VF_NORMAL   0x1 << 1
#define VF_TANGENT  0x1 << 2
#define VF_COLOR    0x1 << 3
#define VF_UV       0x1 << 4
#define VF_BINDEX   0x1 << 5
#define VF_BWEIGHT  0x1 << 6

namespace WOWGE
{
namespace Graphics
{

struct VertexP
{
	static const uint32_t Format = VF_POSITION;
	Maths::Vector3 position;
};

struct VertexPC
{
	static const uint32_t Format = VF_POSITION | VF_COLOR;
	Maths::Vector3 position;
	Maths::Vector4 color;
};

struct VertexPT
{
	static const uint32_t Format = VF_POSITION | VF_UV;
	Maths::Vector3 position;
	Maths::Vector2 uv;
};

struct Vertex
{
	static const uint32_t Format = VF_POSITION | VF_NORMAL | VF_TANGENT | VF_UV;
	Maths::Vector3 position;
	Maths::Vector3 normal;
	Maths::Vector3 tangent;
	Maths::Vector2 uv;
};

struct BoneVertex
{
	static const uint32_t Format = VF_POSITION | VF_NORMAL | VF_TANGENT | VF_UV | VF_BINDEX | VF_BWEIGHT;
	Maths::Vector3 position;
	Maths::Vector3 normal;
	Maths::Vector3 tangent;
	Maths::Vector2 uv;
	int boneIndex[4] = {};
	float boneWeight[4] = {};
};

} //namespace Graphics
} //namespace WOWGE

#endif // !INCLUDED_GRAPHICS_VERTEXTYPES_H
