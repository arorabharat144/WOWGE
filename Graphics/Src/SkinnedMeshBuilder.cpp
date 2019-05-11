#include "Precompiled.h"
#include "SkinnedMeshBuilder.h"

using namespace WOWGE;
using namespace WOWGE::Graphics;

void SkinnedMeshBuilder::CreateCylinder(SkinnedMesh& mesh, uint32_t stacks, uint32_t slices, float height, float radius)
{
	std::vector<BoneVertex> vertices;
	std::vector<uint32_t> indices;

	//
	// Build Stacks
	//

	float stackHeight{ height / stacks }; //Height = 1.0f

	//Amount to increment radius as we move up each stack
	//level from bottom to top
	float radiusStep{ (radius - radius) / stacks }; //Top radius = 0.5, Bottom radius = 0.5
	uint32_t ringCount{ stacks + 1 };

	float dTheta{ 2.0f * 3.141592653589793238462643f / slices };

	BoneVertex boneVertex;
	boneVertex.boneIndex[0] = 0;
	boneVertex.boneIndex[1] = 1;
	boneVertex.boneIndex[2] = 2;
	boneVertex.boneIndex[3] = 3;

	uint32_t numberOfBones{ 4 };
	float bonePositions[4]{ -height / 2.0f, -height / 4.0f, 0.0f , height / 4.0f };

	//Compute vertices for each stack ring starting at 
	// the bottom and moving up
	for (uint32_t i{ 0 }; i < ringCount; ++i)
	{
		float y{ -0.5f * height + i * stackHeight }; //Height = 1.0f
		float r{ radius + i * radiusStep }; //Bottom radius = 0.5

		//Vertices of ring
		for (uint32_t j{ 0 }; j <= slices; ++j)
		{
			float c{ cos(j * dTheta) };
			float s{ sin(j * dTheta) };

			boneVertex.position = { r * c, y, r * s };

			boneVertex.uv = { static_cast<float>(j) / slices, 1.0f - static_cast<float>(i) / stacks };

			boneVertex.tangent = { -s, 0.0f, c };

			boneVertex.normal = { Maths::Cross(boneVertex.tangent,{ 0.0f, -1.0f, 0.0f }) };

		//Bones
		if (y >= bonePositions[0] && y < bonePositions[1])
		{
			float h{ Maths::Absolute(bonePositions[0] - bonePositions[1]) };
			float pos{ y - bonePositions[0] };

			boneVertex.boneWeight[0] = (h - pos) / h;
			boneVertex.boneWeight[1] = pos / h;
			boneVertex.boneWeight[2] = 0.0f;
			boneVertex.boneWeight[3] = 0.0f;
		}
		else if(y >= bonePositions[1] && y < bonePositions[2])
		{
			float h{ Maths::Absolute(bonePositions[1] - bonePositions[2]) };
			float pos{ y - bonePositions[1] };

			boneVertex.boneWeight[0] = 0.0f;
			boneVertex.boneWeight[1] = (h - pos) / h;
			boneVertex.boneWeight[2] = pos / h;
			boneVertex.boneWeight[3] = 0.0f;
		}
		else if (y >= bonePositions[2] && y < bonePositions[3])
		{
			float h{ Maths::Absolute(bonePositions[2] - bonePositions[3]) };
			float pos{ y - bonePositions[2] };

			boneVertex.boneWeight[0] = 0.0f;
			boneVertex.boneWeight[1] = 0.0f;
			boneVertex.boneWeight[2] = (h - pos) / h;
			boneVertex.boneWeight[3] = pos / h;
		}
		else if(y > bonePositions[3] && y <= height)
		{
			boneVertex.boneWeight[0] = 0.0f;
			boneVertex.boneWeight[1] = 0.0f;
			boneVertex.boneWeight[2] = 0.0f;
			boneVertex.boneWeight[3] = 1.0f;
		}

		vertices.push_back(boneVertex);
		}
	}

	// Add one because we duplicate the first and last boneVertex per ring
	// since the texture coordinates are different
	uint32_t ringVertexCount{ slices + 1 };

	// Compute indices for each stack
	for (uint32_t i{ 0 }; i < stacks; ++i)
	{
		for (uint32_t j{ 0 }; j < slices; ++j)
		{
			indices.push_back(i * ringVertexCount + j);
			indices.push_back((i + 1) * ringVertexCount + j);
			indices.push_back((i + 1) * ringVertexCount + j + 1);
			indices.push_back(i * ringVertexCount + j);
			indices.push_back((i + 1) * ringVertexCount + j + 1);
			indices.push_back(i * ringVertexCount + j + 1);
		}
	}

	//Build cylinder top cap
	uint32_t baseIndex{ static_cast<uint32_t>(vertices.size()) };

	float y{ 0.5f * height };//Height = 1.0

	// Duplicate cap ring vertices because the texture coordinates
	// and normals differ.
	for (uint32_t i{ 0 }; i <= slices; ++i)
	{
		float x{ radius * cos(i*dTheta) };
		float z{ radius * sin(i*dTheta) };

		// Scale down by the height to try and make top cap
		// texture coord area proportional to base.
		float u{ x / height + 0.5f }; //Height = 1.0
		float v{ z / height + 0.5f }; //Height = 1.0

		boneVertex.position = { x, y, z };
		boneVertex.normal = { 0.0f, 1.0f, 0.0f };
		boneVertex.tangent = { 1.0f, 0.0f, 0.0f };
		boneVertex.uv = { u,v };

		boneVertex.boneWeight[0] = 0.0f;
		boneVertex.boneWeight[1] = 0.0f;
		boneVertex.boneWeight[2] = 0.0f;
		boneVertex.boneWeight[3] = 1.0f;

		vertices.push_back(boneVertex);
	}

	//Cap center boneVertex
	boneVertex.position = { 0.0f, y, 0.0f };
	boneVertex.normal = { 0.0f, 1.0f, 0.0f };
	boneVertex.tangent = { 1.0f, 0.0f, 0.0f };
	boneVertex.uv = { 0.5f, 0.5f };

	vertices.push_back(boneVertex);

	//Index of center boneVertex
	uint32_t centerIndex{ static_cast<uint32_t>(vertices.size() - 1) };

	for (uint32_t i{ 0 }; i < slices; ++i)
	{
		indices.push_back(centerIndex);
		indices.push_back(baseIndex + i + 1);
		indices.push_back(baseIndex + i);
	}

	//Bottom cap code
	baseIndex = static_cast<uint32_t>(vertices.size());
	y = -0.5f * height; //Height = 1.0
	// Duplicate cap ring vertices because the texture coordinates
	// and normals differ.
	for (uint32_t i = 0; i <= slices; ++i)
	{
		float x{ radius * sin(i * dTheta) }; // Bottom radius = 0.5
		float z{ radius * cos(i * dTheta) }; // Bottom radius = 0.5
		// Scale down by the height to try and make top cap
		// texture coord area proportional to base.
		float u{ x / (height) + 0.5f };
		float v{ z / (height) + 0.5f };

		boneVertex.position = { x, y, z };
		boneVertex.normal = { 0.0f, -1.0f, 0.0f };
		boneVertex.tangent = { -1.0f, 0.0f, 0.0f };
		boneVertex.uv = { u, v };

		boneVertex.boneWeight[0] = 1.0f;
		boneVertex.boneWeight[1] = 0.0f;
		boneVertex.boneWeight[2] = 0.0f;
		boneVertex.boneWeight[3] = 0.0f;

		vertices.push_back(boneVertex);
	}

	// Cap center boneVertex.
	boneVertex.position = { 0.0f, y, 0.0f };
	boneVertex.normal = { 0.0f, -1.0f, 0.0f };
	boneVertex.tangent = { -1.0f, 0.0f, 0.0f };
	boneVertex.uv = { 0.5f, 0.5f };

	vertices.push_back(boneVertex);

	// Index of center boneVertex.
	centerIndex = static_cast<uint32_t>(vertices.size()) - 1;
	for (uint32_t i = 0; i < slices; ++i)
	{
		indices.push_back(centerIndex);
		indices.push_back(baseIndex + i + 1);
		indices.push_back(baseIndex + i);
	}

	const uint32_t numVertices{ static_cast<uint32_t>(vertices.size()) };
	const uint32_t numIndices{ static_cast<uint32_t>(indices.size()) };

	mesh.Allocate(numVertices, numIndices);

	for (uint32_t i{ 0 }; i < numVertices; ++i)
	{
		mesh.GetVertex(i) = vertices[i];
	}

	for (uint32_t i{ 0 }; i < numIndices; ++i)
	{
		mesh.GetIndex(i) = indices[i];
	}
}

void SkinnedMeshBuilder::CreatePlane(SkinnedMesh& mesh)
{
	mesh.Allocate(4, 6);
	/**/mesh.GetVertex(0) = /*0*/{ { 0.0f, 1.0f, 0.0f },{ 0.0f, 0.0f, -1.0f },{ 1.0f, 0.0f, 0.0f },{ 0.0f, 0.0f } , {0}, {0.0f} };
	/**/mesh.GetVertex(1) = /*1*/{ { 1.0f, 0.0f, 0.0f },{ 0.0f, 0.0f, -1.0f },{ 1.0f, 0.0f, 0.0f },{ 1.0f, 1.0f } , {0}, {0.0f} };
	/**/mesh.GetVertex(2) = /*2*/{ { 0.0f, 0.0f, 0.0f },{ 0.0f, 0.0f, -1.0f },{ 1.0f, 0.0f, 0.0f },{ 0.0f, 1.0f } , {0}, {0.0f} };
	/**/mesh.GetVertex(3) = /*3*/{ { 1.0f, 1.0f, 0.0f },{ 0.0f, 0.0f, -1.0f },{ 1.0f, 0.0f, 0.0f },{ 1.0f, 0.0f } , {0}, {0.0f} };

	/**/mesh.GetIndex(0) = 0; mesh.GetIndex(1) = 1; mesh.GetIndex(2) = 2;
	/**/mesh.GetIndex(3) = 0; mesh.GetIndex(4) = 3; mesh.GetIndex(5) = 1;
}