#include "Precompiled.h"

#include "MatrixStack.h"

using namespace WOWGE;
using namespace WOWGE::Graphics;

MatrixStack::MatrixStack()
{

}

MatrixStack::~MatrixStack()
{

}

void MatrixStack::PushMatrix(const Maths::Matrix44& matrix)
{
	if (mStack.empty())
	{
		mStack.push(matrix);
		return;
	}
	mStack.push(matrix * mStack.top());
}

void MatrixStack::PopMatrix()
{
	mStack.pop();
}

Maths::Matrix44 MatrixStack::GetMatrixTranspose() const
{
	return Maths::Transpose(mStack.top());
}