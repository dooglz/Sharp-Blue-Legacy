#pragma once
#include "../Font.h"
#include <map>

namespace Engine{
	namespace VITA{
	class VITA_Font: public Font{
	public:
		void init(){};
		void shutdown(){};

		//! Renders a single character to the specified buffer at the specified position
		void render(const int buffer, const unsigned int character, const float X, const float Y){};

		//! Renders a string of text to the specified buffer at the specified position
		void renderString(const int buffer, const char* characters, const float X, const float Y){};

		//! Sets the color of the font renderer
		void setColour(float R, float G, float B, float A){};

		//! Sets the size of the font renderer
		void setSize(float Size){};

		 VITA_Font(){};
		~VITA_Font(){};
	};
	}
}
