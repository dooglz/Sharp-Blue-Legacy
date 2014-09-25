#pragma once
#include "Event_Manager.h"
namespace Engine{
	namespace PS3{
		class PS3EventManager: public EventManager{

		protected:

		public:
			void processEvents();
			void init();
		};
	}
}