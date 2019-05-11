#ifndef INCLUDED_GRAPHICS_CAMERANODE_H
#define INCLUDED_GRAPHICS_CAMERANODE_H

//====================================================================================================
// Filename:	CameraNode.h
// Description:	Class for a camera node.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include <Math\Inc\Matrix44.h>
#include <Math\Inc\Vector4.h>
#include "Camera.h"
#include "ConstantBuffer.h"
#include "SceneNode.h"

//====================================================================================================
// Forward Declarations
//====================================================================================================

//class Camera;

//====================================================================================================
// Class Declarations
//====================================================================================================

namespace WOWGE
{
namespace Graphics
{
	class CameraNode : public SceneNode
	{
	public:
		struct ConstantBuffer
		{
			Maths::Matrix44 matView[2];
			Maths::Matrix44 matProjection[2];
			Maths::Vector4 viewPosition[2];
		};

		CameraNode();
		~CameraNode();

		void Initialize(uint32_t bindindex = 0);
		void Terminate();

		void SetCamera(Camera* camera, uint32_t index = 0) { mCameras[index] = camera; }

	private:
		void OnUpdate(float deltaTime) override;
		void OnRender() override;

	private:
		TypedConstantBuffer<ConstantBuffer> mConstantBuffer;

		Camera* mCameras[2];

		uint32_t mBindIndex;
	};

} //namespace Graphics
} //namespace WOWGE

#endif //INCLUDED_GRAPHICS_CAMERANODE_H