#include "Precompiled.h"
#include "HeightMap.h"

using namespace WOWGE;
using namespace WOWGE::Graphics;

namespace
{
	inline uint32_t GetIndex(uint32_t x, uint32_t y, uint32_t width)
	{
		return x + (y * width);
	}
}

HeightMap::HeightMap():
	mHeightValues(nullptr),
	mColumns(0),
	mRows(0)
{

}

HeightMap::~HeightMap()
{
	ASSERT(mHeightValues == nullptr, "[HeightMap] Height values not freed.");
}

void HeightMap::GenerateFromRAW(const char* fileName, uint32_t columns, uint32_t rows)
{
	ASSERT(mHeightValues == nullptr, "[HeightMap] HeightMap is already initialized.");
	ASSERT(columns > 0, "[HeightMap] Invalid value for columns");
	ASSERT(rows > 0, "[HeightMap] Invalid value for rows");

	FILE* file = nullptr;
	fopen_s(&file, fileName, "rb");
	ASSERT(file != nullptr, "[HeightMap] Failed to load heightmap %s", fileName);

	fseek(file, 0, SEEK_END);
	uint32_t len = (uint32_t)ftell(file);
	fseek(file, 0, SEEK_SET);
	ASSERT(len == columns * rows, "[Heightmap] Invalid heightmap dimension. Expected %ux%u = %u bytes", columns, rows, columns * rows);

	//Allocate memory for height data
	mHeightValues = new float[columns * rows];
	mColumns = columns;
	mRows = rows;

	for (uint32_t y = 0; y < rows; ++y)
	{
		for (uint32_t x = 0; x < columns; ++x)
		{
			const uint32_t index = GetIndex(x, y, columns);
			mHeightValues[index] = fgetc(file) / 255.0f;
		}
	}

	fclose(file);
}

void HeightMap::Clear()
{
	SafeDeleteArray(mHeightValues);
	mColumns = 0;
	mRows = 0;
}

float HeightMap::GetHeight(uint32_t x, uint32_t y) const
{
	ASSERT(mHeightValues != nullptr, "[HeightMap] Failed to get height value. Height map is empty");
	ASSERT(x < mColumns&&y < mRows, "[HeightMap] Failed to get height value. Invalid coordinate (%u, %u)", x, y);
	return mHeightValues[GetIndex(x, y, mColumns)];
}

