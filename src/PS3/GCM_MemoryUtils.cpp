#include "GCM_MemoryUtils.h"
#include <stdlib.h>
namespace Engine{
	namespace GCM{
		uint32_t GCM_MemoryUtils::localHeapStart;

		void * GCM_MemoryUtils::localMemoryAlloc(const uint32_t size)
		{
			uint32_t currentHeap = localHeapStart;
			localHeapStart += (size + 1023) & (~1023);
			return (void *)currentHeap;
		}

		void * GCM_MemoryUtils::localMemoryAlign(const uint32_t alignment, const uint32_t size)
		{
			localHeapStart = (localHeapStart + alignment - 1) & (~(alignment - 1));
			return (void *)(localMemoryAlloc(size));
		}
	}
}