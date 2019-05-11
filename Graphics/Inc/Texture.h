#ifndef INCLUDED_GRAPHICS_TEXTURE_H
#define INCLUDED_GRAPHICS_TEXTURE_H

namespace WOWGE
{
namespace Graphics {

class Texture
{
public:
	Texture();
	~Texture();

	void Initialize(const char* filename);
	void Initialize(const void* data, uint32_t width, uint32_t height);
	void Terminate();

	void BindVS(uint32_t slot) const;
	void BindPS(uint32_t slot) const;

	void UnbindVS(uint32_t slot) const;
	void UnbindPS(uint32_t slot) const;

private:
	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;

	friend class SpriteRenderer;

	ID3D11ShaderResourceView* mShaderResourceView;
};

} // namespace Graphics
} // namespace WOWGE

#endif // #ifndef INCLUDED_GRAPHICS_TEXTURE_H