#pragma once
#include "Base_Platform.h"
namespace Engine{
	namespace OGL{
		class OGL_Platform : public Platform{
		protected:
			void loadDefaultShaders(){};

		public:
			OGL_Platform();
			~OGL_Platform(){};

			void InitDisplay(const unsigned short width, const unsigned short height){};
			void shutdown(){};
		};
	}
}