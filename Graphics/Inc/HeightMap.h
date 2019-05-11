#ifndef INCLUDED_GRAPHICS_HEIGHTMAP_HEADER
#define INCLUDED_GRAPHICS_HEIGHTMAP_HEADER

namespace WOWGE
{
namespace Graphics
{
class HeightMap
{
public:
	HeightMap();
	~HeightMap();

	HeightMap(const HeightMap&) = delete;
	HeightMap& operator=(const HeightMap&) = delete;

	void GenerateFromRAW(const char* fileName, uint32_t columns, uint32_t rows);
	void Clear();

	float GetHeight(uint32_t x, uint32_t y) const;

	uint32_t GetColumns() const { return mColumns; }
	uint32_t GetRows() const { return mRows; }

private:
	float* mHeightValues;
	uint32_t mColumns;
	uint32_t mRows;

};

} //namespace Graphics
} //namespace WOWGE

#endif // !INCLUDED_GRAPHICS_HEIGHTMAP_HEADER
