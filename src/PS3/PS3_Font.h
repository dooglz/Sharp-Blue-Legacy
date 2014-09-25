#pragma once
#include "../Font.h"
#include <cell/font.h>
#include <cell/fontGcm.h>
#include <cell/fontFT.h> //FreeType2
#include <map>

namespace Engine{
namespace PS3{

	class PS3_Font: public Font{
	private:
	struct st_Glyph
	{
		//! TODO
		float BaseScaleX, BaseScaleY;
		
		//! Glyph metadata
		CellFontGlyphMetrics  Metrics;

		//! vertices structure of the glyph
		CellFontVertexesGlyph VertexesGlyph;

		//! Pointer to glyph data in main memory
		CellFontGlyph* glyph;

		//! Character code (utf32)
		uint32_t code;
	};

	//Weird memory functions required by Libfont
	static void* fonts_malloc(void*, uint32_t size);
	static void  fonts_free(void*, void*p);
	static void* fonts_realloc(void*, void* p, uint32_t size);
	static void* fonts_calloc(void*, uint32_t numb, uint32_t blockSize);
	
	//! The Font to use for rendering, we will use a built-in system font
	static CellFont font;

	//! Reference to the CellFont Library
	static const CellFontLibrary* library;

	//! Reference to the CellFont Graphics Library
	static const CellFontGraphics*  fontGraphics;

	//! Array of font render surfaces (one for each render buffer)
	static CellFontRenderSurfaceGcm FontRenderSurfaces[2];

	static CellFontGraphicsDrawContext drawContext;
	static CellFontInitGraphicsConfigGcm fontGcmConfig;

	//Memory buffers for rendering
	static void *fontGcmConfigPool;
	static uint32_t fontVertexBufferSize;
	static uint32_t* fontVertexBuffer;

	//! A map containing all loaded Glyphs
	static std::map<uint32_t,st_Glyph> glyphs;

	//! Loads a glyph into memory + RSX
	static void loadCharacter(uint32_t code);

	//! Returns a st_Glyph* with the specified charter code
	static st_Glyph* find(uint32_t code);


	public:
		void init();
		void shutdown(){};

		//! Renders a single character to the specified buffer at the specified position
		void render(const int buffer, const unsigned int character, const float X, const float Y);

		//! Renders a string of text to the specified buffer at the specified position
		void renderString(const int buffer, const char* characters, const float X, const float Y);

		//! Sets the color of the font renderer
		void setColour(float R, float G, float B, float A);

		//! Sets the size of the font renderer
		void setSize(float Size);

		void flush(){};
	};

}}
