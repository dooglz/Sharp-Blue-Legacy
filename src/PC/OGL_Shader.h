#pragma once
#include <string>
#include "../GMaths.h"

/*! \brief PS3 shader class
 * \ingroup PS3_Engine
 */

namespace Engine{
	namespace OGL{
		class OGL_Shader{

		protected:

			//! A table of paramaters/uniforms within the shader for quick lookups.
			//std :: map <std :: string , CGparameter > _uniforms;
			unsigned int _shaderID;
			unsigned int _shaderType; //GL_VERTEX_SHADER or GL_FRAGMENT_SHADER.

		public:

			//! Loads a compiled shader from a file and stores it into Program in an unitialised state
			void LoadBinaryShader(std::string name);

			void LoadSourceShader(std::string name);

			//! Parses a shader stored in a char array and stores it into Program in an unitialised state
			void loadCharArrayShader(const unsigned char * shaderCode);

			//! Initializes the shader program.
			void initProgram(bool storeOnRSX);

			//! Get a shader parameter by name
			//CGparameter GetParameter(std::string name);

			//! Set a named paramter value
			//virtual void  SetParameter (CGparameter param, float * data ) = 0;

			//! Set a named paramter value
			void SetParameter(std::string name, float * data);

			//! Transposes a matrix into a 2d float[] and stores into a named paramter value 
			void SetParameterM(std::string name, Matrix4 & totranpose);

			//! Transposes a matrix into a 2d float[] and stores into a named paramter value 
			//void SetParameterM (CGparameter param, Vectormath::Aos::Matrix4 & totranpose );

			//! The shader program, residing in main memory
			//CGprogram program;

			//! The shader program code
			//void* ucode{};

			unsigned int getID(){ return _shaderID; };
		};

	}
}