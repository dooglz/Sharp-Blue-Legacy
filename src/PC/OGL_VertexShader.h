#pragma once
#include "OGL_Shader.h"
namespace Engine{
	namespace OGL{
		class OGL_VertexShader : public OGL_Shader{

		public:
			OGL_VertexShader();

			//! Set a named paramter value
			//void SetParameter (CGparameter param, float * data );

			//! Resolve position and colour parameter resource indexes.
			void SetDefaultAttributes();


			//These indexes are used for cellGcmSetVertexDataArray() when sending data to the graphics chip.
			//Essentially these represent where in the actual rendering hardware a particular parameter is bound.

			//! Index of position parameter in vertex shader
			unsigned int VERTEX_POSITION_INDEX;

			//! Index of colour parameter in vertex shader
			unsigned int VERTEX_COLOUR_INDEX;
		};
	}
}