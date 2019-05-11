#ifndef INCLUDED_GRAPHICS_SAMPLERNODE_H
#define INCLUDED_GRAPHICS_SAMPLERNODE_H

//====================================================================================================
// Filename:	SamplerNode.h
// Description:	Class for a transform node.
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Sampler.h"
#include "ConstantBuffer.h"
#include "SceneNode.h"

//====================================================================================================
// Class Declarations
//====================================================================================================

namespace WOWGE
{
namespace Graphics
{

class SamplerNode : public SceneNode
{
public:
	SamplerNode();
	~SamplerNode();

	void Initialize(Sampler::Filter filter, Sampler::AddressMode addressMode);
	void Initialize(Sampler::Filter filter, Sampler::AddressMode addressMode, uint32_t index);
	void Terminate();

private:
	void OnRender() override;

private:
	Sampler mpSampler;
	uint32_t mIndex;
};

} //namespace Graphics
} //namespace WOWGE

#endif //INCLUDED_GRAPHICS_SAMPLERNODE_H