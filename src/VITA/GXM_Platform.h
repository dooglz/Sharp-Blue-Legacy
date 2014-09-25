#pragma once
#include "Base_Platform.h"
namespace Engine{
	namespace GXM{
		class GXM_Platform : public Platform{
		protected:
			void loadDefaultShaders();

		public:
			GXM_Platform();
			~GXM_Platform();

			void InitDisplay(const unsigned short width, const unsigned short height);
			void shutdown();
		};
	}
}