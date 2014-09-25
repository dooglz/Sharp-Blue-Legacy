#pragma once
#include "../Event_Manager.h"

namespace Engine{
	namespace VITA{
	class VITA_EventManager: public EventManager{

	protected:

	public:
		void processEvents(){};
		void init(){};
		~VITA_EventManager(){};
	};
	}
}