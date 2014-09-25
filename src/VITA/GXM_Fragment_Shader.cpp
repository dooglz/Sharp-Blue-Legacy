#include "GXM_Fragment_Shader.h"

namespace Engine{
	namespace GXM{

		void GXM_Fragment_Shader::createPatch(
					const SceGxmOutputRegisterFormat outputFormat,
					const SceGxmMultisampleMode multisampleMode,
					const SceGxmBlendInfo* blendInfo,
					const SceGxmProgram* vertexProgram)
		{
			printf("Fragemnt Shader %s, Patching \n",_name.c_str());
			SceGxmFragmentProgram* patchedProgram;
			int err = sceGxmShaderPatcherCreateFragmentProgram(
				shaderPatcher,
				_patcherID,
				outputFormat,
				multisampleMode,
				blendInfo,
				vertexProgram,
				&patchedProgram);
			SCE_ASSERT(err == SCE_OK);
			_patches.push_back(patchedProgram);
		}

		SceGxmFragmentProgram* GXM_Fragment_Shader::getPatchedProgram()
		{
			return _patches.front();
		}
}}