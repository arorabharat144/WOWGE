#ifndef INCLUDED_GRAPHICS_VERTEXSHADER_H
#define INCLUDED_GRAPHICS_VERTEXSHADER_H

#include <Graphics/Inc/GraphicsSystem.h>

namespace WOWGE
{
namespace Graphics
{

class VertexShader
{
public:
	VertexShader();
	~VertexShader();

	void Initialize(const wchar_t* fileName, const char* entryPoint, const char* shaderModel, uint32_t vertexFormat);
	void InitializeWithoutFile(const char* shader, const char* entryPoint, const char* shaderModel, uint32_t vertexFormat);
	void Terminate();
	void Bind();

private:
	ID3D11VertexShader* mVertexShader;
	ID3D11InputLayout* mInputLayout;
};

} //namespace Graphics
} //namespace WOWGE

#endif // !INCLUDED_GRAPHICS_VERTEXSHADER_H
