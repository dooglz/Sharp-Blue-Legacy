#include "GCM_FragmentShader.h"
namespace Engine{
	namespace GCM{
		// Reloads the instruction cache of the fragment shader
		void GCM_FragmentShader::UpdateShaderVariables()
		{
			//Either this or cellGcmSetFragmentProgram() should be called when a parameter changes.
			//However this command is more efficient as it only changes the parameters in memory.
			cell::Gcm::cellGcmSetUpdateFragmentProgramParameter(offset);
		}

		// Set a named paramter value
		void GCM_FragmentShader::SetParameter(CGparameter param, float * data)
		{
			cell::Gcm::cellGcmSetFragmentProgramParameter(program, param, data, offset);
		}
	}
}