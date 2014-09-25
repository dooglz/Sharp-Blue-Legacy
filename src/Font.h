#pragma once
//

namespace Engine{
	class Font{
	public:
		virtual void init(){};
		virtual void shutdown(){};

		//! Renders a single character to the specified buffer at the specified position
		virtual void render(const int buffer, const unsigned int character, const float X, const float Y){};

		//! Renders a string of text to the specified buffer at the specified position
		virtual void renderString(const int buffer, const char* characters, const float X, const float Y){};

		//! Sets the color of the font renderer
		virtual void setColour(float R, float G, float B, float A){};

		//! Sets the size of the font renderer
		virtual void setSize(float Size){};

		virtual void flush(){};
		virtual ~Font(){};
	};

}