#include "Precompiled.h"
#include "MeshBuilder.h"

using namespace WOWGE;
using namespace WOWGE::Graphics;

void MeshBuilder::CreateCube(Mesh& mesh)
{
	mesh.Allocate(24, 36);

	//VERTICES
	//FRONT FACE
	/**/
	/**/mesh.GetVertex(0) = /*0*/{ { -0.5f,  0.5f, -0.5f }	,{ 0.0f, 0.0f, -1.0f },{ 1.0f, 0.0f, 0.0f },{ 0.0f, 0.0f } };
	/**/mesh.GetVertex(1) = /*1*/{ { 0.5f,  0.5f, -0.5f }	,{ 0.0f, 0.0f, -1.0f },{ 1.0f, 0.0f, 0.0f },{ 1.0f, 0.0f } };
	/**/mesh.GetVertex(2) = /*2*/{ { 0.5f, -0.5f, -0.5f }	,{ 0.0f, 0.0f, -1.0f },{ 1.0f, 0.0f, 0.0f },{ 1.0f, 2.0f } };
	/**/mesh.GetVertex(3) = /*3*/{ { -0.5f, -0.5f, -0.5f }  ,{ 0.0f, 0.0f, -1.0f },{ 1.0f, 0.0f, 0.0f },{ 0.0f, 2.0f } };
	/**/
	/**///RIGHT FACE
	/**/mesh.GetVertex(4) = /*20*/{ { 0.5f,  0.5f,  -0.5f },{ 1.0f, 0.0f, 0.0f },{ 0.0f, 0.0f, 1.0f },{ 0.0f, 0.0f } };
	/**/mesh.GetVertex(5) = /*21*/{ { 0.5f,  0.5f,  0.5f },{ 1.0f, 0.0f, 0.0f },{ 0.0f, 0.0f, 1.0f },{ 2.0f, 0.0f } };
	/**/mesh.GetVertex(6) = /*22*/{ { 0.5f, -0.5f,  0.5f },{ 1.0f, 0.0f, 0.0f },{ 0.0f, 0.0f, 1.0f },{ 2.0f, 2.0f } };
	/**/mesh.GetVertex(7) = /*23*/{ { 0.5f, -0.5f,  -0.5f },{ 1.0f, 0.0f, 0.0f },{ 0.0f, 0.0f, 1.0f },{ 0.0f, 2.0f } };
	/**/
	/**///BACK FACE
	/**/mesh.GetVertex(8) = /*4*/{ { 0.5f,  0.5f, 0.5f },{ 0.0f, 0.0f, 1.0f },{ 1.0f, 0.0f, 0.0f },{ 0.0f, 0.0f } };
	/**/mesh.GetVertex(9) =	/*5*/{ { -0.5f,  0.5f, 0.5f },{ 0.0f, 0.0f, 1.0f },{ 1.0f, 0.0f, 0.0f },{ 2.0f, 0.0f } };
	/**/mesh.GetVertex(10) =	/*6*/{ { -0.5f, -0.5f, 0.5f },{ 0.0f, 0.0f, 1.0f },{ 1.0f, 0.0f, 0.0f },{ 2.0f, 2.0f } };
	/**/mesh.GetVertex(11) =	/*7*/{ { 0.5f, -0.5f, 0.5f },{ 0.0f, 0.0f, 1.0f },{ 1.0f, 0.0f, 0.0f },{ 0.0f, 2.0f } };
	/**/
	/**///LEFT FACE
	/**/mesh.GetVertex(12) = /*16*/{ { -0.5f,  0.5f, 0.5f },{ -1.0f, 0.0f, 0.0f },{ 0.0f, 0.0f, 1.0f },{ 0.0f, 0.0f } };
	/**/mesh.GetVertex(13) = /*17*/{ { -0.5f,  0.5f, -0.5f },{ -1.0f, 0.0f, 0.0f },{ 0.0f, 0.0f, 1.0f },{ 2.0f, 0.0f } };
	/**/mesh.GetVertex(14) = /*18*/{ { -0.5f, -0.5f, -0.5f },{ -1.0f, 0.0f, 0.0f },{ 0.0f, 0.0f, 1.0f },{ 2.0f, 2.0f } };
	/**/mesh.GetVertex(15) = /*19*/{ { -0.5f, -0.5f, 0.5f },{ -1.0f, 0.0f, 0.0f },{ 0.0f, 0.0f, 1.0f },{ 0.0f, 2.0f } };
	/**/
	/**///TOP FACE
	/**/mesh.GetVertex(16) = /*8*/{ { -0.5f, 0.5f,  -0.5f },{ 0.0f, 1.0f, 0.0f },{ 1.0f, 0.0f, 0.0f },{ 0.0f, 0.0f } };
	/**/mesh.GetVertex(17) = /*9*/{ { -0.5f, 0.5f,  0.5f },{ 0.0f, 1.0f, 0.0f },{ 1.0f, 0.0f, 0.0f },{ 2.0f, 0.0f } };
	/**/mesh.GetVertex(18) = /*10*/{ { 0.5f, 0.5f,  0.5f },{ 0.0f, 1.0f, 0.0f },{ 1.0f, 0.0f, 0.0f },{ 2.0f, 2.0f } };
	/**/mesh.GetVertex(19) = /*11*/{ { 0.5f, 0.5f,  -0.5f },{ 0.0f, 1.0f, 0.0f },{ 1.0f, 0.0f, 0.0f },{ 0.0f, 2.0f } };
	/**/
	/**///BOTTOM FACE
	/**/mesh.GetVertex(20) = /*12*/{ { -0.5f, -0.5f, -0.5f },{ 0.0f, -1.0f, 0.0f },{ -1.0f, 0.0f, 0.0f },{ 0.0f, 0.0f } };
	/**/mesh.GetVertex(21) = /*13*/{ { 0.5f, -0.5f, -0.5f },{ 0.0f, -1.0f, 0.0f },{ -1.0f, 0.0f, 0.0f },{ 2.0f, 0.0f } };
	/**/mesh.GetVertex(22) = /*14*/{ { 0.5f, -0.5f, 0.5f },{ 0.0f, -1.0f, 0.0f },{ -1.0f, 0.0f, 0.0f },{ 2.0f, 2.0f } };
	/**/mesh.GetVertex(23) = /*15*/{ { -0.5f, -0.5f, 0.5f },{ 0.0f, -1.0f, 0.0f },{ -1.0f, 0.0f, 0.0f },{ 0.0f, 2.0f } };
	/**/



	//INDICES
	//FRONT FACE
	/**/
	/**/mesh.GetIndex(0) = 0; mesh.GetIndex(1) = 1; mesh.GetIndex(2) = 2;
	/**/mesh.GetIndex(3) = 0; mesh.GetIndex(4) = 2; mesh.GetIndex(5) = 3;
	/**/
	/**///BACK FACE
	/**/mesh.GetIndex(6) = 4; mesh.GetIndex(7) = 5; mesh.GetIndex(8) = 6;
	/**/mesh.GetIndex(9) = 4; mesh.GetIndex(10) = 6; mesh.GetIndex(11) = 7;
	/**/
	/**///TOP FACE
	/**/mesh.GetIndex(12) = 9; mesh.GetIndex(13) = 10; mesh.GetIndex(14) = 11;
	/**/mesh.GetIndex(15) = 9; mesh.GetIndex(16) = 11; mesh.GetIndex(17) = 8;
	/**/
	/**///BOTTOM FACE
	/**/mesh.GetIndex(18) = 12; mesh.GetIndex(19) = 13; mesh.GetIndex(20) = 14;
	/**/mesh.GetIndex(21) = 12; mesh.GetIndex(22) = 14; mesh.GetIndex(23) = 15;
	/**/
	/**///LEFT FACE
	/**/mesh.GetIndex(24) = 16; mesh.GetIndex(25) = 17; mesh.GetIndex(26) = 19;
	/**/mesh.GetIndex(27) = 19; mesh.GetIndex(28) = 17; mesh.GetIndex(29) = 18;
	/**/
	/**///RIGHT FACE
	/**/mesh.GetIndex(30) = 20; mesh.GetIndex(31) = 21; mesh.GetIndex(32) = 22;
	/**/mesh.GetIndex(33) = 20; mesh.GetIndex(34) = 22; mesh.GetIndex(35) = 23;
	/**/

}

void MeshBuilder::CreateSphere(Mesh& mesh, uint32_t stacks, uint32_t slices)
{
	const uint32_t numVertices = ((stacks + 1) * (slices + 1));
	const uint32_t numIndices = stacks * (6 * (slices));

	mesh.Allocate(numVertices, numIndices);

	const float kSliceStep = 2.0f * 3.141592653589793238462643f / (stacks - 1);
	const float kRingStep = 3.141592653589793238462643f / (slices - 1);
	// Fill vertex data
	float uStep = 1.0f / (slices - 1);
	float vStep = 1.0f / stacks;
	uint32_t index = 0;

	for (uint32_t j = 0; j <= slices; ++j)
	{
		const float phi = j * kRingStep;
		for (uint32_t i = 0; i <= stacks; ++i)
		{
			float   theta = i * kSliceStep;
			float   y = cos(phi);
			float   r = sqrt(1.0f - (y * y));
			float   s = sin(theta);
			float   c = cos(theta);
			float   x = r * c;
			float	  z = r * s;
			Maths::Vector3 p(x, y, z);
			Maths::Vector3 n(x, y, z);


			Maths::Vector3 t(-s, 0.0f, c);

			Maths::Vector2 uv((i * uStep), (j * vStep));

			if (i == stacks)
			{
				index = (j* stacks) + i;
				mesh.GetVertex(index) = mesh.GetVertex(index - slices);
				continue;
			}

			index = (j* stacks) + i;

			mesh.GetVertex(index) = { p, n, t, uv };

		}
	}


	uint32_t indicesIndex = 0;
	// Fill index data
	for (uint32_t j = 0; j < slices - 1; ++j)
	{
		for (uint32_t i = 0; i <= stacks; ++i)
		{
			const uint32_t a = i % (stacks + 1);
			const uint32_t b = (i + 1) % (stacks + 1);
			const uint32_t c = j * (stacks + 1);
			const uint32_t d = (j + 1) * (stacks + 1);

			mesh.GetIndex(indicesIndex) = a + c;
			indicesIndex++;
			mesh.GetIndex(indicesIndex) = b + c;
			indicesIndex++;
			mesh.GetIndex(indicesIndex) = a + d;
			indicesIndex++;
			mesh.GetIndex(indicesIndex) = b + c;
			indicesIndex++;
			mesh.GetIndex(indicesIndex) = b + d;
			indicesIndex++;
			mesh.GetIndex(indicesIndex) = a + d;
			indicesIndex++;
		}
	}
}

void MeshBuilder::CreateCylinder(Mesh& mesh, uint32_t stacks, uint32_t slices)
{
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	//
	// Build Stacks
	//

	float stackHeight{ 1.0f / stacks }; //Height = 1.0f

	//Amount to increment radius as we move up each stack
	//level from bottom to top
	float radiusStep{ (0.5f - 0.5f) / stacks }; //Top radius = 0.5, Bottom radius = 0.5
	uint32_t ringCount{ stacks + 1 };

	float dTheta{ 2.0f * 3.141592653589793238462643f / slices };

	Vertex vertex;

	//Compute vertices for each stack ring starting at 
	// the bottom and moving up
	for (uint32_t i{ 0 }; i < ringCount; ++i)
	{
		float y{ -0.5f * 1.0f + i * stackHeight }; //Height = 1.0f
		float r{ 0.5f + i * radiusStep }; //Bottom radius = 0.5

		//Vertices of ring
		for (uint32_t j{ 0 }; j <= slices; ++j)
		{


			float c{ cos(j * dTheta) };
			float s{ sin(j * dTheta) };

			vertex.position = { r * c, y, r * s };

			vertex.uv = { static_cast<float>(j) / slices, 1.0f - static_cast<float>(i) / stacks };
			
			vertex.tangent = { -s, 0.0f, c };

			vertex.normal = { Maths::Cross(vertex.tangent, {0.0f, -1.0f, 0.0f}) };

			vertices.push_back(vertex);
		}
	}

	// Add one because we duplicate the first and last vertex per ring
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

	//Build cylinder top cap, bottom cap
	uint32_t baseIndex{ static_cast<uint32_t>(vertices.size()) };

	float y{ 0.5f * 1.0f };//Height = 1.0
	
	// Duplicate cap ring vertices because the texture coordinates
	// and normals differ.
	for (uint32_t i{ 0 }; i <= slices; ++i)
	{
		float x{ 0.5f * cos(i*dTheta) };
		float z{ 0.5f * sin(i*dTheta) };

		// Scale down by the height to try and make top cap
		// texture coord area proportional to base.
		float u{ x / 1.0f + 0.5f }; //Height = 1.0
		float v{ z / 1.0f + 0.5f }; //Height = 1.0

		vertex.position = { x, y, z };
		vertex.normal = { 0.0f, 1.0f, 0.0f };
		vertex.tangent = { 1.0f, 0.0f, 0.0f };
		vertex.uv = { u,v };

		vertices.push_back(vertex);
	}

	//Cap center vertex
	vertex.position = { 0.0f, y, 0.0f };
	vertex.normal = { 0.0f, 1.0f, 0.0f };
	vertex.tangent = { 1.0f, 0.0f, 0.0f };
	vertex.uv = { 0.5f, 0.5f };

	vertices.push_back(vertex);
	
	//Index of center vertex
	uint32_t centerIndex{ static_cast<uint32_t>(vertices.size() - 1) };

	for (uint32_t i{ 0 }; i < slices; ++i)
	{
		indices.push_back(centerIndex);
		indices.push_back(baseIndex + i + 1);
		indices.push_back(baseIndex + i);
	}

	//Bottom cap code
	baseIndex = static_cast<uint32_t>(vertices.size());
	y = -0.5f * 1.0f; //Height = 1.0
	// Duplicate cap ring vertices because the texture coordinates
	// and normals differ.
	for (uint32_t i = 0; i <= slices; ++i)
	{
		float x{ 0.5f * sin(i * dTheta) }; // Bottom radius = 0.5
		float z{ 0.5f * cos(i * dTheta) }; // Bottom radius = 0.5
		// Scale down by the height to try and make top cap
		// texture coord area proportional to base.
		float u{ x / (1.0f) + 0.5f };
		float v{ z / (1.0f) + 0.5f };

		vertex.position = { x, y, z };
		vertex.normal = { 0.0f, -1.0f, 0.0f };
		vertex.tangent = { -1.0f, 0.0f, 0.0f };
		vertex.uv = { u, v };

		vertices.push_back(vertex);
	}

	// Cap center vertex.
	vertex.position = { 0.0f, y, 0.0f };
	vertex.normal = { 0.0f, -1.0f, 0.0f };
	vertex.tangent = { -1.0f, 0.0f, 0.0f };
	vertex.uv = { 0.5f, 0.5f };

	vertices.push_back(vertex);

	// Index of center vertex.
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

void MeshBuilder::CreateSkySphere(Mesh& mesh, uint32_t stacks, uint32_t slices)
{
	const uint32_t numVertices = ((stacks + 1) * (slices + 1));
	const uint32_t numIndices = stacks * (6 * (slices));

	mesh.Allocate(numVertices, numIndices);

	const float kSliceStep = 2.0f * 3.141592653589793238462643f / (stacks - 1);
	const float kRingStep = 3.141592653589793238462643f / (slices - 1); 
													  // Fill vertex data
	float uStep = 1.0f / (slices - 1);
	float vStep = 1.0f / stacks;
	uint32_t index = 0;

	for (uint32_t j = 0; j <= slices; ++j)
	{
		const float phi = j * kRingStep;
		for (uint32_t i = 0; i <= stacks; ++i)
		{
			float   theta = i * kSliceStep;
			float   y = cos(phi);
			float   r = sqrt(1.0f - (y * y));
			float   s = sin(theta);
			float   c = cos(theta);
			float   x = r * c;
			float	z = r * s;
			Maths::Vector3 p(x, y, z);
			Maths::Vector3 n(-x, -y, -z);
			Maths::Vector3 t(-s, 0.0f, c);
			Maths::Vector2 uv((i *uStep), (j*vStep));

			if (i == stacks)
			{
				index = (j* stacks) + i;
				mesh.GetVertex(index) = mesh.GetVertex(index - slices);
				continue;
			}

			index = (j* stacks) + i;

			mesh.GetVertex(index) = { p, n, t, uv };

		}
	}

	uint32_t indicesIndex = 0;
	// Fill index data
	for (uint32_t j = 0; j < slices - 1; ++j)
	{
		for (uint32_t i = 0; i <= stacks; ++i)
		{
			const uint32_t a = i % (stacks + 1);
			const uint32_t b = (i + 1) % (stacks + 1);
			const uint32_t c = j * (stacks + 1);
			const uint32_t d = (j + 1) * (stacks + 1);

			//When i==0 && j==0
			//TRIANGLE 1
			mesh.GetIndex(indicesIndex) = a + d; //4
			indicesIndex++;
			mesh.GetIndex(indicesIndex) = b + c;
			indicesIndex++;						 //1
			mesh.GetIndex(indicesIndex) = a + c;
			indicesIndex++;						 //0

			//When i==0 && j==0
			//TRIANGLE 2
			mesh.GetIndex(indicesIndex) = a + d; //4
			indicesIndex++;
			mesh.GetIndex(indicesIndex) = b + d;
			indicesIndex++;						 //5
			mesh.GetIndex(indicesIndex) = b + c;
			indicesIndex++;						 //1
		}
	}
}

void MeshBuilder::CreatePostProcessingPlane(Mesh& mesh)
{
	mesh.Allocate(4, 6);
	mesh.GetVertex(0) = /*0*/{ { -1.0f, 1.0f, 0.0f },{ 0.0f, 0.0f, -1.0f },{ 1.0f, 0.0f, 0.0f },{ 0.0f, 0.0f } };
	mesh.GetVertex(1) = /*1*/{ { 1.0f, -1.0f, 0.0f },{ 0.0f, 0.0f, -1.0f },{ 1.0f, 0.0f, 0.0f },{ 1.0f, 1.0f } };
	mesh.GetVertex(2) = /*2*/{ { -1.0f, -1.0f, 0.0f },{ 0.0f, 0.0f, -1.0f },{ 1.0f, 0.0f, 0.0f },{ 0.0f, 1.0f } };
	mesh.GetVertex(3) = /*3*/{ { 1.0f, 1.0f, 0.0f },{ 0.0f, 0.0f, -1.0f },	{ 1.0f, 0.0f, 0.0f },{ 1.0f, 0.0f } };

	mesh.GetIndex(0) = 0; mesh.GetIndex(1) = 1; mesh.GetIndex(2) = 2;
	mesh.GetIndex(3) = 0; mesh.GetIndex(4) = 3; mesh.GetIndex(5) = 1;
}

void MeshBuilder::CreatePlane(Mesh& mesh)
{
	mesh.Allocate(4, 6);
	/**/mesh.GetVertex(0) = /*0*/{ { 0.0f, 1.0f, 0.0f },{ 0.0f, 0.0f, -1.0f },{ 1.0f, 0.0f, 0.0f },{ 0.0f, 0.0f } };
	/**/mesh.GetVertex(1) = /*1*/{ { 1.0f, 0.0f, 0.0f },{ 0.0f, 0.0f, -1.0f },{ 1.0f, 0.0f, 0.0f },{ 3.0f, 3.0f } };
	/**/mesh.GetVertex(2) = /*2*/{ { 0.0f, 0.0f, 0.0f },{ 0.0f, 0.0f, -1.0f },{ 1.0f, 0.0f, 0.0f },{ 0.0f, 3.0f } };
	/**/mesh.GetVertex(3) = /*3*/{ { 1.0f, 1.0f, 0.0f },{ 0.0f, 0.0f, -1.0f },{ 1.0f, 0.0f, 0.0f },{ 3.0f, 0.0f } };

	/**/mesh.GetIndex(0) = 0; mesh.GetIndex(1) = 1; mesh.GetIndex(2) = 2;
	/**/mesh.GetIndex(3) = 0; mesh.GetIndex(4) = 3; mesh.GetIndex(5) = 1;
}