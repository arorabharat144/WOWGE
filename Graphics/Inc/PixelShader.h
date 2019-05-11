#ifndef INCLUDED_GRAPHICS_PIXELSHADER_H
#define INCLUDED_GRAPHICS_PIXELSHADER_H

#include <Graphics/Inc/GraphicsSystem.h>

namespace WOWGE
{
namespace Graphics
{

class PixelShader
{
public:
	PixelShader();
	~PixelShader();

	void Initialize(const wchar_t* fileName);
	//void Initialize(const wchar_t* fileName, const char* entryPoint, const char* shaderModel);
	void Initialize(const wchar_t* shader, const char* entryPoint, const char* shaderModel);
	void InitializeWithoutFile(const char* shader, const char* entryPoint, const char* shaderModel);
	void Terminate();
	void Bind();

private:
	ID3D11PixelShader* mPixelShader;
};

} //namespace Graphics
} //namespace WOWGE


#endif // !INCLUDED_GRAPHICS_PIXELSHADER_H
