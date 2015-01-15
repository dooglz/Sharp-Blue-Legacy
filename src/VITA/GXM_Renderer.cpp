#include <stdio.h>
#include <kernel.h>
#include "GXM_Renderer.h"
#include "GXM_MemoryUtils.h"
#include <display.h>

namespace Engine {
namespace GXM {

// User main thread parameters
extern const char sceUserMainThreadName[] = "libgxm_basic_main_thr";
extern const int sceUserMainThreadPriority = SCE_KERNEL_DEFAULT_PRIORITY_USER;
extern const unsigned int sceUserMainThreadStackSize =
    SCE_KERNEL_STACK_SIZE_DEFAULT_USER_MAIN;

// Define a 1MiB heap for the program
unsigned int sceLibcHeapSize = 1 * 1024 * 1024;

const SceGxmProgram *const GXM_Renderer::clearVertexProgramGxp =
    &_binary_clear_v_gxp_start;
const SceGxmProgram *const GXM_Renderer::clearFragmentProgramGxp =
    &_binary_clear_f_gxp_start;
const SceGxmProgram *const GXM_Renderer::basicVertexProgramGxp =
    &_binary_basic_v_gxp_start;
const SceGxmProgram *const GXM_Renderer::basicFragmentProgramGxp =
    &_binary_basic_f_gxp_start;

SceGxmContext *GXM_Renderer::_context;
SceGxmFragmentProgram *GXM_Renderer::_currentFrag;
SceGxmVertexProgram *GXM_Renderer::_currentVert;

GXM_Renderer::GXM_Renderer(void) {}

void GXM_Renderer::init() {
  int err = 0;
  SceGxmInitializeParams initializeParams;
  memset(&initializeParams, 0, sizeof(SceGxmInitializeParams));
  initializeParams.flags = 0;
  initializeParams.displayQueueMaxPendingCount = DISPLAY_MAX_PENDING_SWAPS;
  initializeParams.displayQueueCallback = displayCallback;
  initializeParams.displayQueueCallbackDataSize = sizeof(DisplayData);
  initializeParams.parameterBufferSize = SCE_GXM_DEFAULT_PARAMETER_BUFFER_SIZE;

  // initialize
  err = sceGxmInitialize(&initializeParams);
  SCE_ASSERT(err == SCE_OK);

  /* ---------------------------------------------------------------------
  3. Create a libgxm context

  Once initialized, we need to create a rendering context to allow to us
  to render scenes on the GPU.  We use the default initialization
  parameters here to set the sizes of the various context ring buffers.
  --------------------------------------------------------------------- */

  // allocate ring buffer memory using default sizes
  SceUID vdmRingBufferUid;
  void *vdmRingBuffer =
      graphicsAlloc(SCE_KERNEL_MEMBLOCK_TYPE_USER_RWDATA_UNCACHE,
                    SCE_GXM_DEFAULT_VDM_RING_BUFFER_SIZE, 4,
                    SCE_GXM_MEMORY_ATTRIB_READ, &vdmRingBufferUid);
  SceUID vertexRingBufferUid;
  void *vertexRingBuffer =
      graphicsAlloc(SCE_KERNEL_MEMBLOCK_TYPE_USER_RWDATA_UNCACHE,
                    SCE_GXM_DEFAULT_VERTEX_RING_BUFFER_SIZE, 4,
                    SCE_GXM_MEMORY_ATTRIB_READ, &vertexRingBufferUid);
  SceUID fragmentRingBufferUid;
  void *fragmentRingBuffer =
      graphicsAlloc(SCE_KERNEL_MEMBLOCK_TYPE_USER_RWDATA_UNCACHE,
                    SCE_GXM_DEFAULT_FRAGMENT_RING_BUFFER_SIZE, 4,
                    SCE_GXM_MEMORY_ATTRIB_READ, &fragmentRingBufferUid);
  SceUID fragmentUsseRingBufferUid;
  uint32_t fragmentUsseRingBufferOffset;
  void *fragmentUsseRingBuffer = fragmentUsseAlloc(
      SCE_GXM_DEFAULT_FRAGMENT_USSE_RING_BUFFER_SIZE,
      &fragmentUsseRingBufferUid, &fragmentUsseRingBufferOffset);

  SceGxmContextParams contextParams;
  memset(&contextParams, 0, sizeof(SceGxmContextParams));
  contextParams.hostMem = malloc(SCE_GXM_MINIMUM_CONTEXT_HOST_MEM_SIZE);
  contextParams.hostMemSize = SCE_GXM_MINIMUM_CONTEXT_HOST_MEM_SIZE;
  contextParams.vdmRingBufferMem = vdmRingBuffer;
  contextParams.vdmRingBufferMemSize = SCE_GXM_DEFAULT_VDM_RING_BUFFER_SIZE;
  contextParams.vertexRingBufferMem = vertexRingBuffer;
  contextParams.vertexRingBufferMemSize =
      SCE_GXM_DEFAULT_VERTEX_RING_BUFFER_SIZE;
  contextParams.fragmentRingBufferMem = fragmentRingBuffer;
  contextParams.fragmentRingBufferMemSize =
      SCE_GXM_DEFAULT_FRAGMENT_RING_BUFFER_SIZE;
  contextParams.fragmentUsseRingBufferMem = fragmentUsseRingBuffer;
  contextParams.fragmentUsseRingBufferMemSize =
      SCE_GXM_DEFAULT_FRAGMENT_USSE_RING_BUFFER_SIZE;
  contextParams.fragmentUsseRingBufferOffset = fragmentUsseRingBufferOffset;

  _context = NULL;
  err = sceGxmCreateContext(&contextParams, &_context);
  SCE_ASSERT(err == SCE_OK);

  // initialize controller data
  memset(&ctrlData, 0, sizeof(ctrlData));

  // message for SDK sample auto test
  printf("## api_libgxm/basic: INIT SUCCEEDED ##\n");

  backBufferIndex = 0;
  frontBufferIndex = 0;
  rotationAngle = 0.0f;

  InitDisplay();
  InitSurfaces();
  GXM_Shader::initShaderPatcher();
  loadDefaultShaders();
}

void GXM_Renderer::assignShader(stMesh *m, std::string name) {
  // TODO
  /*
  m->vertShader = defaultVert;
  m->fragShader = defaultFrag;
  */
}

// Detect and set resolution
void GXM_Renderer::InitDisplay() {

  /* ---------------------------------------------------------------------
  Create a render target
  Finally we create a render target to describe the geometry of the back
  buffers we will render to.  This object is used purely to schedule
  rendering jobs for the given dimensions, the color surface and
  depth/stencil surface must be allocated separately.
  --------------------------------------------------------------------- */

  // set up parameters
  SceGxmRenderTargetParams renderTargetParams;
  memset(&renderTargetParams, 0, sizeof(SceGxmRenderTargetParams));
  renderTargetParams.flags = 0;
  renderTargetParams.width = DISPLAY_WIDTH;
  renderTargetParams.height = DISPLAY_HEIGHT;
  renderTargetParams.scenesPerFrame = 1;
  renderTargetParams.multisampleMode = MSAA_MODE;
  renderTargetParams.multisampleLocations = 0;
  renderTargetParams.driverMemBlock = SCE_UID_INVALID_UID;

  // create the render target
  int err = sceGxmCreateRenderTarget(&renderTargetParams, &_renderTarget);
  SCE_ASSERT(err == SCE_OK);
}

// Create Buffers/surfaces in memory
void GXM_Renderer::InitSurfaces() {
  printf("Creating buffers\n");

  /*Allocate display buffers and sync objects
  We will allocate our back buffers in CDRAM, and create a color
  surface for each of them */

  // allocate memory and sync objects for display buffers
  SceUID displayBufferUid[DISPLAY_BUFFER_COUNT];

  for (uint32_t i = 0; i < DISPLAY_BUFFER_COUNT; ++i) {
    // allocate memory for display
    _surfacesData[i] =
        graphicsAlloc(SCE_KERNEL_MEMBLOCK_TYPE_USER_CDRAM_RWDATA,
                      4 * DISPLAY_STRIDE_IN_PIXELS * DISPLAY_HEIGHT,
                      SCE_GXM_COLOR_SURFACE_ALIGNMENT,
                      SCE_GXM_MEMORY_ATTRIB_READ | SCE_GXM_MEMORY_ATTRIB_WRITE,
                      &displayBufferUid[i]);

    // memset the buffer to black
    for (uint32_t y = 0; y < DISPLAY_HEIGHT; ++y) {
      uint32_t *row =
          (uint32_t *)_surfacesData[i] + y * DISPLAY_STRIDE_IN_PIXELS;
      for (uint32_t x = 0; x < DISPLAY_WIDTH; ++x) {
        row[x] = 0xff000000;
      }
    }

    // initialize a color surface for this display buffer
    err = sceGxmColorSurfaceInit(
        &_surfaces[i], DISPLAY_COLOR_FORMAT, SCE_GXM_COLOR_SURFACE_LINEAR,
        (MSAA_MODE == SCE_GXM_MULTISAMPLE_NONE)
            ? SCE_GXM_COLOR_SURFACE_SCALE_NONE
            : SCE_GXM_COLOR_SURFACE_SCALE_MSAA_DOWNSCALE,
        SCE_GXM_OUTPUT_REGISTER_SIZE_32BIT, DISPLAY_WIDTH, DISPLAY_HEIGHT,
        DISPLAY_STRIDE_IN_PIXELS, _surfacesData[i]);
    SCE_ASSERT(err == SCE_OK);

    // create a sync object that we will associate with this buffer
    err = sceGxmSyncObjectCreate(&_surfacesSync[i]);
    SCE_ASSERT(err == SCE_OK);
  }

  /* ---------------------------------------------------------------------
  6. Allocate a depth buffer

  Note that since this sample renders in a strictly back-to-front order,
  a depth buffer is not strictly required.  However, since it is usually
  necessary to create one to handle partial renders, we will create one
  now.  Note that we do not enable force load or store, so this depth
  buffer will not actually be read or written by the GPU when this
  sample is executed, so will have zero performance impact.
  --------------------------------------------------------------------- */

  // compute the memory footprint of the depth buffer
  const uint32_t alignedWidth = ALIGN(DISPLAY_WIDTH, SCE_GXM_TILE_SIZEX);
  const uint32_t alignedHeight = ALIGN(DISPLAY_HEIGHT, SCE_GXM_TILE_SIZEY);
  uint32_t sampleCount = alignedWidth * alignedHeight;
  uint32_t depthStrideInSamples = alignedWidth;
  if (MSAA_MODE == SCE_GXM_MULTISAMPLE_4X) {
    // samples increase in X and Y
    sampleCount *= 4;
    depthStrideInSamples *= 2;
  } else if (MSAA_MODE == SCE_GXM_MULTISAMPLE_2X) {
    // samples increase in Y only
    sampleCount *= 2;
  }

  // allocate it
  SceUID depthBufferUid;
  void *depthBufferData =
      graphicsAlloc(SCE_KERNEL_MEMBLOCK_TYPE_USER_RWDATA_UNCACHE,
                    4 * sampleCount, SCE_GXM_DEPTHSTENCIL_SURFACE_ALIGNMENT,
                    SCE_GXM_MEMORY_ATTRIB_READ | SCE_GXM_MEMORY_ATTRIB_WRITE,
                    &depthBufferUid);

  // create the SceGxmDepthStencilSurface structure

  err = sceGxmDepthStencilSurfaceInit(
      &_depthSurface, SCE_GXM_DEPTH_STENCIL_FORMAT_S8D24,
      SCE_GXM_DEPTH_STENCIL_SURFACE_TILED, depthStrideInSamples,
      depthBufferData, NULL);
  SCE_ASSERT(err == SCE_OK);
}

void GXM_Renderer::setupFrame() {

  // check control data
  sceCtrlPeekBufferPositive(0, &ctrlData, 1);

  // update triangle angle
  rotationAngle += (3.14159 * 2) / 60.0f;
  if (rotationAngle > (3.14159 * 2))
    rotationAngle -= (3.14159 * 2);

  // set up a 4x4 matrix for a rotation
  float aspectRatio = (float)DISPLAY_WIDTH / (float)DISPLAY_HEIGHT;

  float s = sin(rotationAngle);
  float c = cos(rotationAngle);

  wvpData[0] = c / aspectRatio;
  wvpData[1] = s;
  wvpData[2] = 0.0f;
  wvpData[3] = 0.0f;

  wvpData[4] = -s / aspectRatio;
  wvpData[5] = c;
  wvpData[6] = 0.0f;
  wvpData[7] = 0.0f;

  wvpData[8] = 0.0f;
  wvpData[9] = 0.0f;
  wvpData[10] = 1.0f;
  wvpData[11] = 0.0f;

  wvpData[12] = 0.0f;
  wvpData[13] = 0.0f;
  wvpData[14] = 0.0f;
  wvpData[15] = 1.0f;

  // start rendering to the main render target
  sceGxmBeginScene(_context, 0, _renderTarget, NULL, NULL,
                   _surfacesSync[backBufferIndex], &_surfaces[backBufferIndex],
                   &_depthSurface);
}

void GXM_Renderer::clearSurface() {
  // set clear shaders
  sceGxmSetVertexProgram(_context, blankVertShader->getPatchedProgram());
  sceGxmSetFragmentProgram(_context, blankFragShader->getPatchedProgram());

  // draw the clear triangle
  sceGxmSetVertexStream(_context, 0, clearVertices);
  sceGxmDraw(_context, SCE_GXM_PRIMITIVE_TRIANGLES, SCE_GXM_INDEX_FORMAT_U16,
             clearIndices, 3);

  // render the rotating triangle
  sceGxmSetVertexProgram(_context, basicVertShader->getPatchedProgram());
  sceGxmSetFragmentProgram(_context, basicFragShader->getPatchedProgram());

  // set the vertex program constants
  void *vertexDefaultBuffer;
  sceGxmReserveVertexDefaultUniformBuffer(_context, &vertexDefaultBuffer);
  sceGxmSetUniformDataF(vertexDefaultBuffer,
                        basicVertShader->getParameter("wvp"), 0, 16, wvpData);

  // draw the spinning triangle
  sceGxmSetVertexStream(_context, 0, basicVertices);
  sceGxmDraw(_context, SCE_GXM_PRIMITIVE_TRIANGLES, SCE_GXM_INDEX_FORMAT_U16,
             basicIndices, 3);
}

// Switch which buffer is active(being rendered on) and currentyl displayed
void GXM_Renderer::swapBuffers() {
  // end the scene on the main render target, submitting rendering work to the
  // GPU
  sceGxmEndScene(_context, NULL, NULL);

  // performance analysis (PA) heartbeat to notify end of frame
  sceGxmPadHeartbeat(&_surfaces[backBufferIndex],
                     _surfacesSync[backBufferIndex]);

  // queue the display swap for this frame
  DisplayData displayData;
  displayData.address = _surfacesData[backBufferIndex];
  sceGxmDisplayQueueAddEntry(
      _surfacesSync[frontBufferIndex], // front buffer is OLD buffer
      _surfacesSync[backBufferIndex],  // back buffer is NEW buffer
      &displayData);

  // update buffer indices
  frontBufferIndex = backBufferIndex;
  backBufferIndex = (backBufferIndex + 1) % DISPLAY_BUFFER_COUNT;
}

// Set the active vertex and fragment shader
void GXM_Renderer::SetCurrentShader(SceGxmVertexProgram &vert,
                                    SceGxmFragmentProgram &frag) {

  _currentFrag = &frag;
  _currentVert = &vert;

  sceGxmSetVertexProgram(_context, &vert);
  sceGxmSetFragmentProgram(_context, &frag);
}

// Initialises viewport (coordinate scaling)
void GXM_Renderer::SetViewport() {}

void GXM_Renderer::loadDefaultShaders() {
  puts("Making Default Shaders\n");

  // register programs with the patcher
  blankVertShader = new GXM_Vertex_Shader("clearVertex", clearVertexProgramGxp);
  blankVertShader->registerWithPatcher();
  blankFragShader =
      new GXM_Fragment_Shader("clearFragment", clearFragmentProgramGxp);
  blankFragShader->registerWithPatcher();
  basicVertShader = new GXM_Vertex_Shader("basicVertex", basicVertexProgramGxp);
  basicVertShader->registerWithPatcher();
  basicFragShader =
      new GXM_Fragment_Shader("basicFragment", basicFragmentProgramGxp);
  basicFragShader->registerWithPatcher();

  // create clear vertex format
  SceGxmVertexAttribute clearVertexAttributes[1];
  SceGxmVertexStream clearVertexStreams[1];
  clearVertexAttributes[0].streamIndex = 0;
  clearVertexAttributes[0].offset = 0;
  clearVertexAttributes[0].format = SCE_GXM_ATTRIBUTE_FORMAT_F32;
  clearVertexAttributes[0].componentCount = 2;
  clearVertexAttributes[0].regIndex = sceGxmProgramParameterGetResourceIndex(
      blankVertShader->getParameter("aPosition"));
  clearVertexStreams[0].stride = sizeof(ClearVertex);
  clearVertexStreams[0].indexSource = SCE_GXM_INDEX_SOURCE_INDEX_16BIT;

  // create clear patched programs
  blankVertShader->createPatch(clearVertexAttributes, 1, clearVertexStreams, 1);
  blankFragShader->createPatch(SCE_GXM_OUTPUT_REGISTER_FORMAT_UCHAR4, MSAA_MODE,
                               NULL, blankVertShader->unpatchedProgram);

  // create the clear triangle vertex/index data
  SceUID clearVerticesUid;
  SceUID clearIndicesUid;
  clearVertices = (ClearVertex *)graphicsAlloc(
      SCE_KERNEL_MEMBLOCK_TYPE_USER_RWDATA_UNCACHE, 3 * sizeof(ClearVertex), 4,
      SCE_GXM_MEMORY_ATTRIB_READ, &clearVerticesUid);
  clearIndices = (uint16_t *)graphicsAlloc(
      SCE_KERNEL_MEMBLOCK_TYPE_USER_RWDATA_UNCACHE, 3 * sizeof(uint16_t), 2,
      SCE_GXM_MEMORY_ATTRIB_READ, &clearIndicesUid);

  // The clear vertex & index data is static, we allocate and write the data
  // here.
  clearVertices[0].x = -1.0f;
  clearVertices[0].y = -1.0f;
  clearVertices[1].x = 3.0f;
  clearVertices[1].y = -1.0f;
  clearVertices[2].x = -1.0f;
  clearVertices[2].y = 3.0f;

  clearIndices[0] = 0;
  clearIndices[1] = 1;
  clearIndices[2] = 2;

  // Create the programs and data for the spinning triangle

  // create shaded triangle vertex format
  SceGxmVertexAttribute basicVertexAttributes[2];
  SceGxmVertexStream basicVertexStreams[1];
  basicVertexAttributes[0].streamIndex = 0;
  basicVertexAttributes[0].offset = 0;
  basicVertexAttributes[0].format = SCE_GXM_ATTRIBUTE_FORMAT_F32;
  basicVertexAttributes[0].componentCount = 3;
  basicVertexAttributes[0].regIndex = sceGxmProgramParameterGetResourceIndex(
      basicVertShader->getParameter("aPosition"));
  basicVertexAttributes[1].streamIndex = 0;
  basicVertexAttributes[1].offset = 12;
  basicVertexAttributes[1].format = SCE_GXM_ATTRIBUTE_FORMAT_U8N;
  basicVertexAttributes[1].componentCount = 4;
  basicVertexAttributes[1].regIndex = sceGxmProgramParameterGetResourceIndex(
      basicVertShader->getParameter("aColor"));
  basicVertexStreams[0].stride = sizeof(BasicVertex);
  basicVertexStreams[0].indexSource = SCE_GXM_INDEX_SOURCE_INDEX_16BIT;

  // create shaded triangle shaders
  basicVertShader->createPatch(basicVertexAttributes, 2, basicVertexStreams, 1);
  basicFragShader->createPatch(SCE_GXM_OUTPUT_REGISTER_FORMAT_UCHAR4, MSAA_MODE,
                               NULL, basicVertShader->unpatchedProgram);

  // create shaded triangle vertex/index data
  SceUID basicVerticesUid;
  SceUID basicIndiceUid;
  basicVertices = (BasicVertex *)graphicsAlloc(
      SCE_KERNEL_MEMBLOCK_TYPE_USER_RWDATA_UNCACHE, 3 * sizeof(BasicVertex), 4,
      SCE_GXM_MEMORY_ATTRIB_READ, &basicVerticesUid);
  basicIndices = (uint16_t *)graphicsAlloc(
      SCE_KERNEL_MEMBLOCK_TYPE_USER_RWDATA_UNCACHE, 3 * sizeof(uint16_t), 2,
      SCE_GXM_MEMORY_ATTRIB_READ, &basicIndiceUid);

  basicVertices[0].x = -0.5f;
  basicVertices[0].y = -0.5f;
  basicVertices[0].z = 0.0f;
  basicVertices[0].color = 0xff0000ff;
  basicVertices[1].x = 0.5f;
  basicVertices[1].y = -0.5f;
  basicVertices[1].z = 0.0f;
  basicVertices[1].color = 0xff00ff00;
  basicVertices[2].x = -0.5f;
  basicVertices[2].y = 0.5f;
  basicVertices[2].z = 0.0f;
  basicVertices[2].color = 0xffff0000;

  basicIndices[0] = 0;
  basicIndices[1] = 1;
  basicIndices[2] = 2;
}

void GXM_Renderer::renderMesh(stMesh *msh, Matrix4 mvp) {}

void GXM_Renderer::displayCallback(const void *callbackData) {
  SceDisplayFrameBuf framebuf;
  memset(&framebuf, 0x00, sizeof(SceDisplayFrameBuf));

  // callbackData is a copy of the sceGxmDisplayQueueAddEntry parameters
  const DisplayData *displayData = (const DisplayData *)callbackData;

  // Rebuild the parameters
  framebuf.size = sizeof(SceDisplayFrameBuf);
  framebuf.base = displayData->address;
  framebuf.pitch = DISPLAY_STRIDE_IN_PIXELS;
  framebuf.pixelformat = DISPLAY_PIXEL_FORMAT;
  framebuf.width = DISPLAY_WIDTH;
  framebuf.height = DISPLAY_HEIGHT;
  // Swap to the new buffer on the next VSYNC
  int err =
      sceDisplaySetFrameBuf(&framebuf, SCE_DISPLAY_UPDATETIMING_NEXTVSYNC);
  SCE_ASSERT(err == SCE_OK);

  // Swap to the new buffer on the next VSYNC
  err = sceDisplaySetFrameBuf(&framebuf, SCE_DISPLAY_UPDATETIMING_NEXTVSYNC);
  SCE_ASSERT(err == SCE_OK);
  // Block this callback until the swap has finished
  err = sceDisplayWaitVblankStart();
  SCE_ASSERT(err == SCE_OK);

  // Block this callback until the swap has finished
  err = sceDisplayWaitVblankStart();
  SCE_ASSERT(err == SCE_OK);
}
}
}