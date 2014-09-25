#pragma once
#include "GCM_Shader.h"
namespace Engine{
	namespace GCM{
		class GCM_FragmentShader : public GCM_Shader{

		public:
			//! Set a named paramter value
			void SetParameter(CGparameter param, float * data);

			//! Reloads the instruction cache of the fragment shader
			void UpdateShaderVariables();

			~GCM_FragmentShader(){};
		};
	}
}