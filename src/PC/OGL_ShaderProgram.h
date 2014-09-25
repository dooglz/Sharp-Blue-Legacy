#pragma once
#include "OGL_FragmentShader.h"
#include "OGL_VertexShader.h"
namespace Engine{
	namespace OGL{
		class OGL_ShaderProgram{

		public:
			OGL_ShaderProgram();

			OGL_VertexShader VS;
			OGL_FragmentShader FS;

			std::string name;

			void link();

			void attachShader(OGL_Shader* shader);

			GLuint getID();


		protected:
			GLuint _ID;

		};
	}
}