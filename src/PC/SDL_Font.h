#pragma once
#include "../Font.h"
#include "sdl/SDL_ttf.h"
#include <map>

namespace Engine{
	class SDL_Font: public Font{
	private:
		unsigned int _fontSize;
		std::map<unsigned int, TTF_Font*> _fonts;
		SDL_Surface* _surface_buffer;
		SDL_Color _color;
		unsigned int _VAO;
		unsigned int _VBO;
		unsigned int _gltexture;

		TTF_Font* SDL_Font::getFont(const unsigned int size);
		void SDL_Font::loadfont(const unsigned int size);
	public:
		void init();
		void shutdown();

		//! Renders a single character to the specified buffer at the specified position
		void render(const int buffer, const uint32_t character, const float X, const float Y);

		//! Renders a string of text to the specified buffer at the specified position
		void renderString(const int buffer, const char* characters, const float X, const float Y);

		//! Sets the color of the font renderer
		void setColour(float R, float G, float B, float A);

		//! Sets the size of the font renderer
		void setSize(float Size);

		void apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip);

		void flush();
	};

}