#pragma once
#include <stdlib.h>
namespace Engine{
	namespace GCM{
		class GCM_MemoryUtils{

		public:
			//TODO: description
			static uint32_t localHeapStart;
			//TODO: description
			static void* localMemoryAlloc(const uint32_t size);
			//TODO: description
			static void* localMemoryAlign(const uint32_t alignment, const uint32_t size);
		};
	}
}