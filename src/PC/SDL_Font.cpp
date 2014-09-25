#include "SDL_Font.h"
#include "../Utilities.h"
#include "OGL_Renderer.h"

namespace Engine{

	void SDL_Font::init()
	{
		printf("Font Init \n");
		//Initialize SDL_ttf
		int err = TTF_Init();
		DBG_ASSERT_MSG((err == 0),"TTF_Init() error");
		//Open the font
		loadfont(28);
		_fontSize = 28;
		_color = { 0,0,0 };

		//SDL interprets each pixel as a 32-bit number
		Uint32 rmask, gmask, bmask, amask;
		rmask = 0x000000ff;
		gmask = 0x0000ff00;
		bmask = 0x00ff0000;
		amask = 0xff000000;

		//Create an SDL surface for rendering text to.
		_surface_buffer = SDL_CreateRGBSurface(SDL_SWSURFACE, 1280, 720, 32,rmask, gmask, bmask, amask);

		//Generate VAO
		glGenVertexArrays(1, &_VAO);
		OGL::OGL_Renderer::checkerr();

		//Generate VBO
		glGenBuffers(1, &_VBO);
		OGL::OGL_Renderer::checkerr();

		//Generate glTexture
		glGenTextures(1, &_gltexture);
		OGL::OGL_Renderer::checkerr();
	}

	TTF_Font* SDL_Font::getFont(const unsigned int size)
	{
		std::map<unsigned int, TTF_Font* >::iterator it = _fonts.find(size);
		if (it != _fonts.end()){
			return it->second;
		}
		loadfont(size);
		it = _fonts.find(size);
		if (it != _fonts.end()){
			return it->second;
		}
		//CAN'T FIND FONT SIZE, fallback
		return _fonts.begin()->second;
	}

	void SDL_Font::loadfont(const unsigned int size)
	{
		TTF_Font* font = TTF_OpenFont("fonts/LiberationSans-Regular.ttf", size);
		//_font = TTF_OpenFont("fonts/lazy.ttf", size);
		DBG_ASSERT_FUNC((font != NULL), printf("TTF_OpenFont error:  %s/n", TTF_GetError()));

		_fonts.insert(std::pair<unsigned int, TTF_Font*>(size, font));
	}

	void SDL_Font::shutdown()
	{
		//Free GL elements	
		glDeleteTextures(1, &_gltexture);
		glDeleteBuffers(1, &_VBO);
		glDeleteVertexArrays(1, &_VAO);
		OGL::OGL_Renderer::checkerr();

		SDL_FreeSurface(_surface_buffer);

		//Close the font that was used
		for (std::map<unsigned int, TTF_Font*>::iterator it = _fonts.begin(); it != _fonts.end(); ++it)
		{
			TTF_CloseFont(it->second);
		}
		_fonts.clear();

		//Quit SDL_ttf
		TTF_Quit();
	}


	void SDL_Font::apply_surface(int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL)
	{
		//Holds offsets
		SDL_Rect offset;

		//Get offsets
		offset.x = x;
		offset.y = y;
		//SDL_Texture* sdlTexture; 
		//sdlTexture = SDL_CreateTextureFromSurface(Engine::OGL::OGL_Renderer::getSDLRenderer(), source);
		//Blit

		SDL_BlitSurface(source, clip, destination, &offset);
		//SDL_RenderCopy(Engine::OGL::OGL_Renderer::getSDLRenderer(), sdlTexture, clip, NULL);
	}

		//! Renders a single character to the specified buffer at the specified position
	void SDL_Font::render(const int buffer, const uint32_t character, const float X, const float Y)
	{
		SDL_Surface *text_surface;
		Uint16 chr = (Uint16)character; //TODO: Remove this madness.

		text_surface = TTF_RenderGlyph_Blended(getFont(_fontSize), chr, _color);
		DBG_ASSERT_FUNC((text_surface != NULL), printf("TTF_RenderGlyph_Solid:  %s/n", TTF_GetError()));

		apply_surface(X, Y, text_surface, _surface_buffer);
		SDL_FreeSurface(text_surface);
	}

		//! Renders a string of text to the specified buffer at the specified position
	void SDL_Font::renderString(const int buffer, const char* characters, const float X, const float Y)
	{
		SDL_Surface *text_surface;

		text_surface = TTF_RenderText_Blended(getFont(_fontSize), characters, _color);
		DBG_ASSERT_FUNC((text_surface != NULL), printf("TTF_RenderText_Solid:  %s/n", TTF_GetError()));

		//Engine::OGL::OGL_Renderer::getSurface()
		apply_surface(X, Y, text_surface, _surface_buffer);
		SDL_FreeSurface(text_surface);
	}

		//! Sets the color of the font renderer
	void SDL_Font::setColour(float R, float G, float B, float A){
		_color = { R,G,B,A };
	}

		//! Sets the size of the font renderer
	void SDL_Font::setSize(float Size){
		_fontSize = (int)Size;
	}


	void SDL_Font::flush()
	{
		//Load the texture shader
		int textureShaderProgram = Engine::OGL::OGL_Renderer::getTextureProgram()->getID();
		glUseProgram(textureShaderProgram);
		OGL::OGL_Renderer::checkerr();

		//Coordinates for a plane (x,y,u,v)
		std::vector<glm::vec4> coords;
		coords.push_back(glm::vec4(-1, -1, 0, 1));
		coords.push_back(glm::vec4(-1, 1, 0, 0));
		coords.push_back(glm::vec4(1, 1, 1, 0));
		coords.push_back(glm::vec4(-1, -1, 0, 1));
		coords.push_back(glm::vec4(1, -1, 1, 1));
		coords.push_back(glm::vec4(1, 1, 1, 0));

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		OGL::OGL_Renderer::checkerr();

		//Which gpu texture UNIT to use.
		glActiveTexture(GL_TEXTURE0);
		OGL::OGL_Renderer::checkerr();

		//Bind to texture
		glBindTexture(GL_TEXTURE_2D, _gltexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _surface_buffer->w, _surface_buffer->h, 0, GL_BGRA, GL_UNSIGNED_BYTE, _surface_buffer->pixels);
		OGL::OGL_Renderer::checkerr();
		
		//Send data to Shader
		GLint TEXTURE_UNIFORM = glGetUniformLocation(textureShaderProgram, "texture");
		glUniform1i(TEXTURE_UNIFORM, 0);
		OGL::OGL_Renderer::checkerr();

		GLint TINT_UNIFORM = glGetUniformLocation(textureShaderProgram, "tint");
		float tint[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		glProgramUniform4fv(textureShaderProgram, TINT_UNIFORM, 1, tint);
		OGL::OGL_Renderer::checkerr();

		//Bind VBO
		glBindBuffer(GL_ARRAY_BUFFER, _VBO);
		OGL::OGL_Renderer::checkerr();

		glBufferData(GL_ARRAY_BUFFER, coords.size() * sizeof(glm::vec4), coords.data(), GL_DYNAMIC_DRAW);
		OGL::OGL_Renderer::checkerr();

		//Bind VAO
		glBindVertexArray(_VAO);
		OGL::OGL_Renderer::checkerr();

		//Position
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);

		glBindVertexArray(_VAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, coords.size());

		glDisableVertexAttribArray(0);
		glUseProgram(0);

		//fill the surface
		SDL_FillRect(_surface_buffer, NULL, 0x5555ff);
	}

}