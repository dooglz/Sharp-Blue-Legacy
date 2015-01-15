#include "GXM_Shader.h"
#include "GXM_MemoryUtils.h"

namespace Engine {
namespace GXM {
SceGxmShaderPatcher* GXM_Shader::shaderPatcher;

GXM_Shader::GXM_Shader(const std::string name, const SceGxmProgram* pgrm) {
  _name = name;
  printf("Shader %s, init\n", name.c_str());
  unpatchedProgram = pgrm;
}

void GXM_Shader::registerWithPatcher() {
  printf("Shader %s, registering with Patcher\n", _name.c_str());
  int err = sceGxmShaderPatcherRegisterProgram(shaderPatcher, unpatchedProgram,
                                               &_patcherID);
  SCE_ASSERT(err == SCE_OK);
  // Cache all parameters
  uint32_t params = sceGxmProgramGetParameterCount(unpatchedProgram);
  for (uint32_t i = 0; i < (params); ++i) {
    const SceGxmProgramParameter* p =
        sceGxmProgramGetParameter(unpatchedProgram, i);
    SCE_ASSERT(p && (sceGxmProgramParameterGetCategory(p) ==
                     SCE_GXM_PARAMETER_CATEGORY_ATTRIBUTE));
    const char* pramName = sceGxmProgramParameterGetName(p);
    printf("Shader %s, parsing paramerter: %i - %s\n", _name.c_str(), i,
           pramName);
    // insert into map
    _params.insert(
        std::pair<std::string, const SceGxmProgramParameter*>(pramName, p));
  }
}

const SceGxmProgramParameter* GXM_Shader::getParameter(const std::string name) {
  std::map<std::string, const SceGxmProgramParameter*>::iterator it =
      _params.find(name);
  if (it != _params.end()) {
    // already in map
    return (it->second);
  }
  HALT;
  return NULL;
}

void GXM_Shader::initShaderPatcher() {

  /*First we create a shader patcher instance, using callback functions
  to allow it to allocate and free host memory for internal state.
  We will use the shader patcher's internal heap to handle buffer
  memory and USSE memory for the final programs.  To do this, we
  leave the callback functions as NULL, but provide static memory
  blocks. */

  // set buffer sizes for this sample
  const uint32_t patcherBufferSize = 64 * 1024;
  const uint32_t patcherVertexUsseSize = 64 * 1024;
  const uint32_t patcherFragmentUsseSize = 64 * 1024;

  // allocate memory for buffers and USSE code
  SceUID patcherBufferUid;
  void* patcherBuffer = graphicsAlloc(
      SCE_KERNEL_MEMBLOCK_TYPE_USER_RWDATA_UNCACHE, patcherBufferSize, 4,
      SCE_GXM_MEMORY_ATTRIB_READ | SCE_GXM_MEMORY_ATTRIB_WRITE,
      &patcherBufferUid);
  SceUID patcherVertexUsseUid;
  uint32_t patcherVertexUsseOffset;
  void* patcherVertexUsse = vertexUsseAlloc(
      patcherVertexUsseSize, &patcherVertexUsseUid, &patcherVertexUsseOffset);
  SceUID patcherFragmentUsseUid;
  uint32_t patcherFragmentUsseOffset;
  void* patcherFragmentUsse =
      fragmentUsseAlloc(patcherFragmentUsseSize, &patcherFragmentUsseUid,
                        &patcherFragmentUsseOffset);

  // create a shader patcher
  SceGxmShaderPatcherParams patcherParams;
  memset(&patcherParams, 0, sizeof(SceGxmShaderPatcherParams));
  patcherParams.userData = NULL;
  patcherParams.hostAllocCallback = &patcherHostAlloc;
  patcherParams.hostFreeCallback = &patcherHostFree;
  patcherParams.bufferAllocCallback = NULL;
  patcherParams.bufferFreeCallback = NULL;
  patcherParams.bufferMem = patcherBuffer;
  patcherParams.bufferMemSize = patcherBufferSize;
  patcherParams.vertexUsseAllocCallback = NULL;
  patcherParams.vertexUsseFreeCallback = NULL;
  patcherParams.vertexUsseMem = patcherVertexUsse;
  patcherParams.vertexUsseMemSize = patcherVertexUsseSize;
  patcherParams.vertexUsseOffset = patcherVertexUsseOffset;
  patcherParams.fragmentUsseAllocCallback = NULL;
  patcherParams.fragmentUsseFreeCallback = NULL;
  patcherParams.fragmentUsseMem = patcherFragmentUsse;
  patcherParams.fragmentUsseMemSize = patcherFragmentUsseSize;
  patcherParams.fragmentUsseOffset = patcherFragmentUsseOffset;

  shaderPatcher = NULL;
  int err = sceGxmShaderPatcherCreate(&patcherParams, &shaderPatcher);
  SCE_ASSERT(err == SCE_OK);
}
}
}