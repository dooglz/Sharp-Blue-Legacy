#pragma once
#include <string>
#include <map>
#include "../GMaths.h"
#include <gxm.h>
#include <gxm/shader_patcher.h>

namespace Engine{
	namespace GXM{
		class GXM_Shader{

		protected:
			void patch();
			//! A table of paramaters/uniforms within the shader for quick lookups.
			std::map<std::string, const SceGxmProgramParameter*> _params;
			std::string _name;
			SceGxmShaderPatcherId _patcherID;
		public:
			static SceGxmShaderPatcher* shaderPatcher;
			const SceGxmProgram* unpatchedProgram;

			void registerWithPatcher();
			const SceGxmProgramParameter* getParameter(const std::string name);
			static void initShaderPatcher();
			GXM_Shader(const std::string name, const SceGxmProgram* pgrm);
		};

	}
}