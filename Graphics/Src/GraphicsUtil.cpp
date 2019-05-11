#include "Precompiled.h"
#include "GraphicsUtil.h"

using namespace WOWGE;
using namespace WOWGE::Graphics;

void GraphicsUtil::ComputeNormals(Mesh& mesh)
{
	uint32_t numberOfIndices = mesh.GetIndexCount();
	uint32_t numberOfVertices = mesh.GetVertexCount();
	std::unique_ptr<Maths::Vector3[]> normals = std::make_unique<Maths::Vector3[]>(mesh.GetVertexCount());


	Maths::Vector3 topAdjacentVector;
	Maths::Vector3 rightAdjacentVector;
	Maths::Vector3 normalVector;

	uint32_t currentIndex;
	uint32_t topIndex;
	uint32_t rightIndex;

	uint32_t meshCurrentIndex;
	uint32_t meshTopIndex;
	uint32_t meshRightIndex;

	for (uint32_t i = 0; i < numberOfIndices; i += 3)
	{
		currentIndex = i;
		topIndex = i + 1;
		rightIndex = i + 2;

		meshCurrentIndex = mesh.GetIndex(currentIndex);
		meshTopIndex = mesh.GetIndex(topIndex);
		meshRightIndex = mesh.GetIndex(rightIndex);

		topAdjacentVector = mesh.GetVertex(meshTopIndex).position - mesh.GetVertex(meshCurrentIndex).position;
		rightAdjacentVector = mesh.GetVertex(meshRightIndex).position - mesh.GetVertex(meshCurrentIndex).position;

		normalVector = Maths::Normalize((Maths::Cross(topAdjacentVector, rightAdjacentVector)));

		normals[meshCurrentIndex] += normalVector;
		normals[meshTopIndex] += normalVector;
		normals[meshRightIndex] += normalVector;
	}

	for (uint32_t i = 0; i < numberOfVertices; ++i)
	{
		normals[i] = Maths::Normalize(normals[i]);
		mesh.GetVertex(i).normal = normals[i];
	}
}

void GraphicsUtil::ComputeNormals(Vertex* vertices, uint32_t numVertices, uint32_t* indices, uint32_t numIndices)
{
	std::unique_ptr<Maths::Vector3[]> normals = std::make_unique<Maths::Vector3[]>(numVertices);

	Maths::Vector3 topAdjacentVector;
	Maths::Vector3 rightAdjacentVector;
	Maths::Vector3 normalVector;

	uint32_t currentIndex;
	uint32_t topIndex;
	uint32_t rightIndex;

	uint32_t meshCurrentIndex;
	uint32_t meshTopIndex;
	uint32_t meshRightIndex;

	for (uint32_t i = 0; i < numIndices; i += 3)
	{
		currentIndex = i;
		topIndex = i + 1;
		rightIndex = i + 2;

		meshCurrentIndex = indices[currentIndex];
		meshTopIndex = indices[topIndex];
		meshRightIndex = indices[rightIndex];

		topAdjacentVector = vertices[meshTopIndex].position - vertices[meshCurrentIndex].position;
		rightAdjacentVector = vertices[meshRightIndex].position - vertices[meshCurrentIndex].position;

		normalVector = Maths::Normalize((Maths::Cross(topAdjacentVector, rightAdjacentVector)));

		normals[meshCurrentIndex] += normalVector;
		normals[meshTopIndex] += normalVector;
		normals[meshRightIndex] += normalVector;
	}

	for (uint32_t i = 0; i < numVertices; ++i)
	{
		normals[i] = Maths::Normalize(normals[i]);
		vertices[i].normal = normals[i];
	}
}