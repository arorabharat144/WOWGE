#include "Precompiled.h"
#include "MeshBuffer.h"
#include "Mesh.h"
#include "SkinnedMesh.h"

using namespace WOWGE;
using namespace WOWGE::Graphics;

MeshBuffer::MeshBuffer():
	mVertexBuffer(nullptr),
	mIndexBuffer(nullptr)
{

}

MeshBuffer::~MeshBuffer()
{
	ASSERT(mVertexBuffer == nullptr, "[MeshBuffer] Vertex buffer not released");
	ASSERT(mIndexBuffer == nullptr, "[MeshBuffer] Index buffer not released");
}

void MeshBuffer::Initialize(const void* vertices, uint32_t vertexSize, uint32_t vertexCount, const uint32_t* indices, uint32_t indexCount)
{
	mVertexSize = vertexSize;
	mIndexCount = indexCount;
	mVertexCount = vertexCount;

	auto graphicsSystem = GraphicsSystem::Get();

	////////////////////////////////
	// Create and fill vertex buffer
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = vertexSize * vertexCount; 
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = vertices;

	graphicsSystem->GetDevice()->CreateBuffer(&bufferDesc, &initData, &mVertexBuffer);

	////////////////////////////////
	//Create and fill Index Buffer
	bufferDesc.ByteWidth = sizeof(uint32_t) * indexCount;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	initData.pSysMem = indices;
	graphicsSystem->GetDevice()->CreateBuffer(&bufferDesc, &initData, &mIndexBuffer);

}

void MeshBuffer::Initialize(const void* vertices, uint32_t vertexSize, uint32_t vertexCount)
{
	mVertexCount = vertexCount;

	auto graphicsSystem = GraphicsSystem::Get();

	////////////////////////////////
	// Create and fill vertex buffer
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = vertexSize * vertexCount; 
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = vertices;

	graphicsSystem->GetDevice()->CreateBuffer(&bufferDesc, &initData, &mVertexBuffer);

}

void MeshBuffer::Initialize(const Mesh& mesh)
{
	Initialize(mesh.GetVertices(), sizeof(Vertex), mesh.GetVertexCount(), mesh.GetIndices(), mesh.GetIndexCount());
}

void MeshBuffer::Initialize(const SkinnedMesh& skinnedMesh)
{
	Initialize(skinnedMesh.GetVertices(), sizeof(BoneVertex), skinnedMesh.GetVertexCount(), skinnedMesh.GetIndices(), skinnedMesh.GetIndexCount());
}

void MeshBuffer::Terminate()
{
	SafeRelease(mIndexBuffer);
	SafeRelease(mVertexBuffer);
}

void MeshBuffer::Render() const
{
	auto graphicsSystem = GraphicsSystem::Get();

	// Set vertex buffer
	UINT stride = mVertexSize; 
	UINT offset = 0;
	graphicsSystem->GetContext()->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset); //UNCOMMENT THIS

	// Set primitive topology
	graphicsSystem->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//DRAW USING INDEX BUFFER
	if (mIndexBuffer != nullptr)
	{
		//Set Index Buffer
		GraphicsSystem::Get()->GetContext()->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		//Draw Indexed Mesh
		GraphicsSystem::Get()->GetContext()->DrawIndexed(mIndexCount, 0, 0);
	}
	else //Draw it raw, without the index buffer
	{
		GraphicsSystem::Get()->GetContext()->Draw(mVertexCount, 0);
	}
}


