#include "GCM_VertexShader.h"
namespace Engine{
	namespace GCM{
		// Set a named paramter value
		void GCM_VertexShader::SetParameter(CGparameter param, float * data)
		{
			cell::Gcm::cellGcmSetVertexProgramParameter(param, data);
		}

		// Resolve position and colour parameter resource indexes.
		void GCM_VertexShader::SetDefaultAttributes()
		{
			//GetParameter() could be used, but would put the parameter result in the map, which could be unnecessary.

			CGparameter position_param = cellGcmCgGetNamedParameter(program, "position");
			//All vertex shaders should have a position parameter
			//DBG_ASSERT(position_param);

			CGparameter colour_param = cellGcmCgGetNamedParameter(program, "color");

			//Get the index of the vertex attribute that will be set for the vertex shader
			//Turn parameters into resources, These are used for cellGcmSetVertexDataArray();
			if (position_param)
			{
				VERTEX_POSITION_INDEX = cellGcmCgGetParameterResource(program, position_param) - CG_ATTR0;
			}

			if (colour_param)
			{
				VERTEX_COLOUR_INDEX = cellGcmCgGetParameterResource(program, colour_param) - CG_ATTR0;
			}
		}
	}
}