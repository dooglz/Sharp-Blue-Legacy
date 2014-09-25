#pragma once
namespace Engine{
	class EventManager{

	protected:

	public:
		virtual void processEvents(){};
		virtual void init(){};
		virtual ~EventManager(){};
	};
}