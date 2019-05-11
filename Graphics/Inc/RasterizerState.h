#ifndef INCLUDED_GRAPHICS_RASTERIZERSTATE_H
#define INCLUDED_GRAPHICS_RASTERIZERSTATE_H

#include "Types.h"

namespace WOWGE
{
namespace Graphics
{

class RasterizerState
{
public:
	RasterizerState();
	~RasterizerState();

	RasterizerState(const RasterizerState&) = delete;
	RasterizerState& operator=(const RasterizerState&) = delete;

	void Initialize(CullMode cullMode, FillMode fillMode);
	void Terminate();

	void Set();
	void Clear();

private:
	ID3D11RasterizerState* mRasterizerState;
};

} //namespace WOWGE
} //namespace Graphics

#endif // !INCLUDED_GRAPHICS_RASTERIZERSTATE_H
