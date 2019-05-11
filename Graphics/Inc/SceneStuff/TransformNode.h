#ifndef INCLUDED_GRAPHICS_TRANSFORMNODE_H
#define INCLUDED_GRAPHICS_TRANSFORMNODE_H

//====================================================================================================
// Filename:	TransformNode.h
// Description:	Class for a transform node.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "ConstantBuffer.h"
#include "SceneNode.h"
#include "Transform.h"
#include "Math\Inc\Matrix44.h"

#include "MatrixStack.h"

//====================================================================================================
// Forward Declarations
//====================================================================================================

//class MatrixStack;

//====================================================================================================
// Class Declarations
//====================================================================================================

namespace WOWGE
{
namespace Graphics
{

class TransformNode : public SceneNode
{
public:
	struct ConstantBuffer
	{
		Maths::Matrix44 matWorld;
	};

	TransformNode(MatrixStack& ms);
	~TransformNode();

	void Initialize(uint32_t bindIndex = 3);
	void Terminate();

	Transform& GetTransform() { return mTransform; }

private:
	void OnUpdate(float deltaTime) override;
	void OnPreRender() override;
	void OnRender() override;
	void OnPostRender() override;

private:
	TypedConstantBuffer<ConstantBuffer> mConstantBuffer;

	MatrixStack& mMatrixStack;

	Transform mTransform;
	uint32_t mBindIndex;
};

} //namespace Graphics
} //namespace WOWGE

#endif //INCLUDED_GRAPHICS_TRANSFORMNODE_H