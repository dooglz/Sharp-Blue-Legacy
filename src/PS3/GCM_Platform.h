#pragma once
#include "Base_Platform.h"
namespace Engine{
	namespace GCM{
		class GCM_Platform : public Platform{
		protected:
			void loadDefaultShaders();

		public:
			GCM_Platform();
			~GCM_Platform();

			void InitDisplay(const unsigned short width, const unsigned short height);
			void shutdown();
		};
	}
}