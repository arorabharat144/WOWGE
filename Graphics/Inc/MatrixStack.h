#ifndef INCLUDED_GRAPHICS_MATRIXSTACK_H
#define INCLUDED_GRAPHICS_MATRIXSTACK_H

#include "Core\Inc\Common.h"

namespace WOWGE
{
namespace Graphics
{
class MatrixStack
{
public:
	MatrixStack();
	~MatrixStack();

	void PushMatrix(const Maths::Matrix44& matrix);
	void PopMatrix();

	Maths::Matrix44 GetMatrixTranspose() const;
private:
	std::stack<Maths::Matrix44> mStack;
};

} //namespace Graphics
} //namespace WOWGE

#endif // !INCLUDED_GRAPHICS_MATRIXSTACK_H
