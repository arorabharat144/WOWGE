#ifndef INCLUDED_AI_MEMORYRECORD_H
#define INCLUDED_AI_MEMORYRECORD_H

namespace WOWGE
{
namespace AI {

class MemoryRecord
{
public:
	MemoryRecord()
		: lastRecordedTime(0.0f)
		, importance(0.0)
	{}

	virtual ~MemoryRecord() {}

	float lastRecordedTime;
	Maths::Vector3 lastLocation;
	float importance;
};

using MemoryRecords = std::list<std::unique_ptr<MemoryRecord>>;

} // namespace AI
} // namespace WOWGE

#endif // #include INCLUDED_AI_MEMORYRECORD_H