#pragma once
#include "GCM_Shader.h"
namespace Engine{
	namespace GCM{
		class GCM_VertexShader : public GCM_Shader{

		public:

			//! Set a named paramter value
			void SetParameter(CGparameter param, float * data);

			//! Resolve position and colour parameter resource indexes.
			void SetDefaultAttributes();


			//These indexes are used for cellGcmSetVertexDataArray() when sending data to the graphics chip.
			//Essentially these represent where in the actual rendering hardware a particular parameter is bound.

			//! Index of position parameter in vertex shader
			uint32_t VERTEX_POSITION_INDEX;

			//! Index of colour parameter in vertex shader
			uint32_t VERTEX_COLOUR_INDEX;

			virtual ~GCM_VertexShader(){};
		};
	}
}