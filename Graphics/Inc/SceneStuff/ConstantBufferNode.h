#ifndef INCLUDED_GRAPHICS_CONSTANTBUFFERNODE_H
#define INCLUDED_GRAPHICS_CONSTANTBUFFERNODE_H

//====================================================================================================
// Filename:	ConstantBufferNode.h
// Description:	Class for a constant buffer node.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include <Math\Inc\Matrix44.h>
#include <Math\Inc\Vector4.h>
#include "ConstantBuffer.h"
#include "SceneNode.h"

//====================================================================================================
// Forward Declarations
//====================================================================================================

//====================================================================================================
// Class Declarations
//====================================================================================================

namespace WOWGE
{
namespace Graphics
{

template<class T>
class ConstantBufferNode : public SceneNode
{
public:
	ConstantBufferNode();
	~ConstantBufferNode();

	void Initialize(uint32_t bindindex = 0);
	void Terminate();

	T& GetData();

private:
	void OnUpdate(float deltaTime) override;
	void OnRender() override;

private:
	T mConstants;

	TypedConstantBuffer<T> mConstantBuffer;

	uint32_t mBindIndex;
};

template<class T>
ConstantBufferNode<T>::ConstantBufferNode() :
	mBindIndex(0)
{
	// Empty
}

//----------------------------------------------------------------------------------------------------

template<class T>
ConstantBufferNode<T>::~ConstantBufferNode()
{
	Terminate();
}

//----------------------------------------------------------------------------------------------------
template<class T>
void ConstantBufferNode<T>::Initialize(uint32_t bindindex)
{
	mBindIndex = bindindex;
	mConstantBuffer.Initialize();
}
//----------------------------------------------------------------------------------------------------

template<class T>
void ConstantBufferNode<T>::Terminate()
{
	mConstantBuffer.Terminate();
}

//----------------------------------------------------------------------------------------------------

template<class T>
T& ConstantBufferNode<T>::GetData()
{
	return mConstants;
}

//----------------------------------------------------------------------------------------------------

template<class T>
void ConstantBufferNode<T>::OnUpdate(float deltaTime)
{
}

//----------------------------------------------------------------------------------------------------

template<class T>
void ConstantBufferNode<T>::OnRender()
{
	mConstantBuffer.Set(mConstants);

	mConstantBuffer.BindVS(mBindIndex);
	mConstantBuffer.BindPS(mBindIndex);
}

} //namespace Graphics
} //namespace WOWGE

#endif //INCLUDED_GRAPHICS_CONSTANTBUFFERNODE_H