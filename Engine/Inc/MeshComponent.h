#ifndef INCLUDED_ENGINE_MESHCOMPONENT_H
#define INCLUDED_ENGINE_MESHCOMPONENT_H

#include "Component.h"

namespace WOWGE
{
namespace Engine
{

class TransformComponent;

class MeshComponent : public Component
{
public:
	META_CLASS_DECLARE

	MeshComponent();
	~MeshComponent() override;

	void Initialize() override;
	void Terminate() override;

	void Render() override;
private:

	const TransformComponent* mTransformComponent;

	std::string mMeshType;
	std::string mTexture;
	std::string mVertexShader;
	std::string mPixelShader;

	struct TransformData
	{
		Maths::Matrix44 world;
		Maths::Matrix44 view;
		Maths::Matrix44 projection;
	};

	using TransformCB = Graphics::TypedConstantBuffer<TransformData>;

	std::unique_ptr<Graphics::MeshBuffer> mMeshBuffer;
	std::unique_ptr<TransformCB> mConstantBuffer;
	std::unique_ptr<Graphics::Sampler> mSampler;
};

} //namespace Engine
} //namespace WOWGE

#endif // !INCLUDED_ENGINE_MESHCOMPONENT_H
