#include "PS3_Font.h"
#include "../Utilities.h"
#include <sysutil/sysutil_sysparam.h> 
#include <cell/sysmodule.h>
#include "GCM_MemoryUtils.h"
#include "GCM_Renderer.h"
#include <cell/gcm.h>

namespace Engine{
namespace PS3{
const CellFontLibrary* PS3_Font::library;
const CellFontGraphics*  PS3_Font::fontGraphics;
void* PS3_Font::fontGcmConfigPool;

CellFont PS3_Font::font;
CellFontRenderSurfaceGcm PS3_Font::FontRenderSurfaces[2];
CellFontGraphicsDrawContext PS3_Font::drawContext;
CellFontInitGraphicsConfigGcm PS3_Font::fontGcmConfig;

uint32_t PS3_Font::fontVertexBufferSize;
uint32_t* PS3_Font::fontVertexBuffer;

std::map<uint32_t, PS3_Font::st_Glyph> PS3_Font::glyphs;

void PS3_Font::init(){
	//load the libfont modules
	int err = cellSysmoduleLoadModule(CELL_SYSMODULE_FS); //libfs (file I/O)
	DBG_ASSERT_FUNC((err == 0), printf("CELL_SYSMODULE_FS module failed! error: %x\n", err));

	err = cellSysmoduleLoadModule(CELL_SYSMODULE_FONT); //libfont(font rendering)
	DBG_ASSERT_FUNC((err == 0), printf("CELL_SYSMODULE_FONT module failed! error: %x\n", err));

	err = cellSysmoduleLoadModule(CELL_SYSMODULE_FREETYPE); //FreeType2 (low-level font access)
	DBG_ASSERT_FUNC((err == 0), printf("CELL_SYSMODULE_FREETYPE module failed! error: %x\n", err));

	err = cellSysmoduleLoadModule(CELL_SYSMODULE_FONTFT); //Interface between libfont and FreeType2
	DBG_ASSERT_FUNC((err == 0), printf("CELL_SYSMODULE_FONTFT module failed ! error: %x\n", err));

	//#---Initializing libfont---#

	// buffer to cache accesses to font files
	static uint32_t FontFileCache[(1024 * 1024) / 4];
	// An Array to store custom fonts, we don't use any, so size=1
	static CellFontEntry UserFontEntrys[1];     
	CellFontConfig config;
	config.FileCache.buffer = &FontFileCache[0];
	config.FileCache.size = 1 * 1024 * 1024;
	config.userFontEntrys = &UserFontEntrys[0];	//Starting address for custom font storage
	config.userFontEntryMax = 0; //No custom fonts, only system fonts
	err = cellFontInit(&config); // Initialize libfont
	DBG_ASSERT_FUNC((err == 0), printf("Initialize libfont module failed! error: %x\n", err));

	//#--- Initializing the Font Interface Library ---#

	CellFontLibraryConfigFT ftConfig;
	CellFontLibraryConfigFT_initialize(&ftConfig);
	ftConfig.MemoryIF.Object = NULL;
	ftConfig.MemoryIF.Malloc = fonts_malloc;
	ftConfig.MemoryIF.Free = fonts_free;
	ftConfig.MemoryIF.Realloc = fonts_realloc;
	ftConfig.MemoryIF.Calloc = fonts_calloc;
	err = cellFontInitLibraryFreeType(&ftConfig, &library);
	DBG_ASSERT_FUNC((err == 0), printf(" cellFontInitLibraryFreeType failed! error: %x\n", err));

	//#--- Open a Font Set ---#

	CellFontType fontType;
	fontType.type = CELL_FONT_TYPE_DEFAULT_GOTHIC_JP_SET;
	fontType.map = CELL_FONT_MAP_UNICODE;
	err = cellFontOpenFontset(library, &fontType, &font);
	DBG_ASSERT_FUNC((err == 0), printf("cellFontOpenFontset failed! error: %x\n", err));

	//Set Scale for Font, Your text layout will depend on this.
	cellFontSetScalePixel(&font, 1.0f, 1.0f);  // Set font scale

	//#--- Prepare the GCM font Renderer ---# 
	
	//Config pool
	fontGcmConfigPool = NULL;
	void *fontGcmLocalConfigPool = NULL;
	//Allocate 1MB of main memory to the pool
	err = sys_memory_allocate(1024 * 1024, SYS_MEMORY_PAGE_SIZE_1M, (sys_addr_t*)(int)&fontGcmConfigPool);
	DBG_ASSERT_FUNC((err == 0), printf("sys_memory_allocate failed! error: %x\n", err));
	DBG_ASSERT_FUNC((fontGcmConfigPool != NULL), printf("error mallocing fontGcmConfigPool\n"));
	//Align the memory
	fontGcmLocalConfigPool = GCM::GCM_MemoryUtils::localMemoryAlign(128, 128 * 1024);
	DBG_ASSERT_FUNC((fontGcmLocalConfigPool != NULL), printf("error aligning fontGcmLocalConfigPool\n"));
	//Map the Main memory pool, so the RSX can use it.
	uint32_t offset;
	err = cellGcmMapMainMemory(fontGcmConfigPool, 1024 * 1024, &offset);
	DBG_ASSERT_FUNC((err == 0), printf("cellGcmMapMainMemory failed! error: %x\n", err));

	// link the GCM font Renderer to available memory locations
	CellFontInitGraphicsConfigGcm_initialize(&fontGcmConfig);
	fontGcmConfig.GraphicsMemory.address = fontGcmLocalConfigPool;
	fontGcmConfig.GraphicsMemory.size = 128 * 1024;
	fontGcmConfig.MappedMainMemory.address = fontGcmConfigPool;
	fontGcmConfig.MappedMainMemory.size = 1024 * 1024;

	// Initialize the GCM font Renderer
	err = cellFontInitGraphicsGcm(&fontGcmConfig, &fontGraphics);
	DBG_ASSERT_FUNC((err == 0), printf("cellFontInitGraphicsGcm failed! error: %x\n", err));
	printf("cell Font Graphics GCM Started! \n");

	fontVertexBufferSize = ((1024 * 1024) - 64 * 1024);
	fontVertexBuffer = (uint32_t*)((uint8_t*)fontGcmConfigPool + 64 * 1024);

	err = cellFontGraphicsSetupDrawContext(fontGraphics, &drawContext);
	DBG_ASSERT_FUNC((err == 0), printf("cellFontGraphicsSetupDrawContext failed! error: %x\n", err));

	// Link the GCM font Renderer to our front and back buffers
	err = cellFontRenderSurfaceGcmInit(&FontRenderSurfaces[0], &Engine::GCM::GCM_Renderer::_surfaces[0]);
	DBG_ASSERT_FUNC((err == 0), printf("cellFontRenderSurfaceGcmInit failed! error: %x\n", err));
	err = cellFontRenderSurfaceGcmInit(&FontRenderSurfaces[1], &Engine::GCM::GCM_Renderer::_surfaces[1]);
	DBG_ASSERT_FUNC((err == 0), printf("cellFontRenderSurfaceGcmInit failed! error: %x\n", err));
	
	//Set defaults
	
	setColour(1, 1, 1, 1);
	setSize(100);
}

// Renders a string of text to the specified buffer at the specified position
void PS3_Font::renderString(const int buffer, const char* characters, const float X, const float Y)
{
	float x = X;
	uint8_t length = std::strlen(characters);
	for (int i = 0; i < length; i++)
	{
		// Render a Character
		render(buffer, characters[i], x, Y);
		// Move to the right
		float w, h;
		// Get the current font size set on the context
		cellFontGraphicsGetScalePixel(&drawContext, &w, &h);
		float rescaleX = (w*1.0); //Depends on cellFontSetScalePixel() being 1.0f.
		st_Glyph* glyph = find(characters[i]);
		x += (rescaleX * glyph->Metrics.Horizontal.advance);
	}
}

// Renders a single character to the specified buffer at the specified position
void PS3_Font::render(int buffer, uint32_t character, float X, float Y){
	int err = cellFontGraphicsGcmSetDrawGlyph(gCellGcmCurrentContext, &FontRenderSurfaces[buffer], X, Y, &find(character)->VertexesGlyph, &drawContext);
	DBG_ASSERT_FUNC((err == 0), printf("cellFontGraphicsGcmSetDrawGlyph failed! error: %x\n", err));
}

// Returns a st_Glyph* with the specified charter code
PS3_Font::st_Glyph* PS3_Font::find(uint32_t code){
	std::map<uint32_t, st_Glyph > ::iterator it = glyphs.find(code);
	if (it != glyphs.end()) {
		//already in map
		return &(it->second);
	}else{
		//not loaded, load.
		loadCharacter(code);
		//search one more time
		it = glyphs.find(code);
		if (it != glyphs.end())
		{
			return &(it->second);
		}else{
			//DBG_HALT;
			return NULL;
		}
	}
}

// Sets the color of the font renderer
void PS3_Font::setColour(float R, float G, float B, float A)
{
	float colour[4];
	colour[0] = R; colour[1] = G; colour[2] = B; colour[3] = A;
	cellFontGraphicsSetFontRGBA(&drawContext, colour);
	cellFontGraphicsSetLineRGBA(&drawContext, colour);
	cellFontGraphicsSetDrawType(&drawContext, CELL_FONT_GRAPHICS_DRAW_TYPE_COLOR);
}

// Sets the size of the font renderer
void PS3_Font::setSize(float Size)
{
	cellFontGraphicsSetScalePixel(&drawContext, Size, Size);
}

// Loads a glyph into memory + RSX
void PS3_Font::loadCharacter(uint32_t code){
	//check to see if already loaded
	std::map<uint32_t, st_Glyph>::iterator it = glyphs.find(code);
	if (it != glyphs.end()) {
		return;	//already in map
	}

	float maxScale = 128.0f;
	float distance;
	st_Glyph glyph;
	CellFontGlyph* glyphA;
	CellFontVertexesGlyph VertexesGlyphA;
	uint32_t dataSize;

	// Generate Glyph
	int err = cellFontGenerateCharGlyph(&font, code, &glyphA);
	if ((err != 0)){
		printf("Couldn't load character code %i, error: %x\n",code, err);
		return;
	}
	DBG_ASSERT_FUNC((err == 0), printf("cellFontGenerateCharGlyph failed! error: %x\n", err));

	// Get Outline Control Distance
	err = cellFontGlyphGetOutlineControlDistance(glyphA, maxScale, CELL_FONT_GLYPH_OUTLINE_CONTROL_DISTANCE_DEFAULT, &distance);
	DBG_ASSERT_FUNC((err == 0), printf("cellFontGlyphGetOutlineControlDistance failed! error: %x\n", err));

	// Generate VertexesGlyphs
	err = cellFontGlyphSetupVertexesGlyph(glyphA, distance, fontVertexBuffer, fontVertexBufferSize, &VertexesGlyphA, &dataSize);
	DBG_ASSERT_FUNC((err == 0), printf("cellFontGlyphSetupVertexesGlyph failed! error: %x\n", err));
	fontVertexBuffer = (uint32_t*)((uint8_t*)fontVertexBuffer + dataSize);
	fontVertexBufferSize -= dataSize;
	if (fontVertexBufferSize <= 0){
		printf("fontVertexBuffer is full!\n");
	}

	// Store
	glyph.code = code;
	glyph.glyph = glyphA;
	glyph.VertexesGlyph = VertexesGlyphA;
	err = cellFontGetCharGlyphMetrics(&font, code, &glyph.Metrics);
	DBG_ASSERT_FUNC((err == 0), printf("cellFontGetCharGlyphMetrics failed! error: %x\n", err));
	glyphs.insert(std::make_pair(code, glyph));

	printf("Loaded new glyph, code:%u, Glyph address: %p, Vertex Address %p\n", code, glyphA, VertexesGlyphA.data);
}


void* PS3_Font::fonts_malloc(void*obj, uint32_t size)
{
	(void)obj;
	return malloc(size);
}
void PS3_Font::fonts_free(void*obj, void*p)
{
	(void)obj;
	free(p);
}
void* PS3_Font::fonts_realloc(void*obj, void* p, uint32_t size)
{
	(void)obj;
	return realloc(p, size);
}
void* PS3_Font::fonts_calloc(void*obj, uint32_t numb, uint32_t blockSize)
{
	(void)obj;
	return calloc(numb, blockSize);
}
}}