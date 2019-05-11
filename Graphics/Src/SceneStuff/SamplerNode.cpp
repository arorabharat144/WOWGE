//====================================================================================================
// Filename:	SamplerNode.cpp
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Graphics\Src\Precompiled.h"

#include "Graphics\Inc\SceneStuff\SamplerNode.h"

using namespace WOWGE;
using namespace WOWGE::Graphics;

//====================================================================================================
// Class Definitions
//====================================================================================================

SamplerNode::SamplerNode()
{
	// Empty
}

//----------------------------------------------------------------------------------------------------

SamplerNode::~SamplerNode()
{
	mpSampler.Terminate();
}

//----------------------------------------------------------------------------------------------------

void SamplerNode::Initialize(Sampler::Filter filter, Sampler::AddressMode addressMode)
{
	mIndex = 0;
	// Create the sample state
	mpSampler.Initialize(filter, addressMode);
}

void SamplerNode::Initialize(Sampler::Filter filter, Sampler::AddressMode addressMode, uint32_t index)
{
	mIndex = index;
	mpSampler.Initialize(filter, addressMode);
}

//----------------------------------------------------------------------------------------------------

void SamplerNode::Terminate()
{
	mpSampler.Terminate();
}

//----------------------------------------------------------------------------------------------------

void SamplerNode::OnRender()
{
	//mGraphicsSystem.GetContext()->PSSetSamplers(0, 1, &mpSampler);

	mpSampler.BindPS(mIndex);
}