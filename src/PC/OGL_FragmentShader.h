#pragma once
#include "OGL_Shader.h"
namespace Engine{
	namespace OGL{
		class OGL_FragmentShader : public OGL_Shader{

		public:
			OGL_FragmentShader();
			//! Set a named paramter value
			//void SetParameter (CGparameter param, float * data );

			//! Reloads the instruction cache of the fragment shader
			void UpdateShaderVariables();
		};
	}
}