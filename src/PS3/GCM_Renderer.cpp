#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include <sys/process.h>
#include <sysutil/sysutil_sysparam.h>
#include <cell/sysmodule.h>

#include <cell/gcm.h>

#include "GCM_Renderer.h"
#include "GCM_MemoryUtils.h"

namespace Engine{
	namespace GCM{
		// Avoid compiling and including external shader files
		// I've included a very very basic vertex shader
		// and pixel shader binary - placed at the bottom of the
		// file - in later practicals, we'll show how to compile and
		// include your own custom shaders
		extern unsigned char fs_basic[208];
		extern unsigned char vs_basic[672];
		using namespace cell::Gcm;


		// The array of render surfaces.
		CellGcmSurface GCM_Renderer::_surfaces[_numberOfSurfaces];
		// A reference to the current Fragment shader in use
		GCM_FragmentShader * GCM_Renderer::_currentFrag;
		// A reference to the current Vertex shader in use
		GCM_VertexShader * GCM_Renderer::_currentVert;
		// A reference to the default Fragment shader
		GCM_FragmentShader * GCM_Renderer::defaultFrag;
		// A reference to the default Vertex shader
		GCM_VertexShader * GCM_Renderer::defaultVert;

		GCM_Renderer::GCM_Renderer(void)
		{

		}


		void GCM_Renderer::init()
		{
			GCM_MemoryUtils::localHeapStart = 0;
			int err = 0;
			err = cellGcmInit(COMMAND_SIZE, BUFFER_SIZE, memalign(1024 * 1024, BUFFER_SIZE));
			DBG_ASSERT_MSG(err == CELL_OK, "cellGcmInit failed !");
			InitDisplay();
			InitSurfaces();
			loadDefaultShaders();
		}


		void GCM_Renderer::assignShader(stMesh* m, std::string name)
		{
			//TODO
			m->vertShader = defaultVert;
			m->fragShader = defaultFrag;
		}


		// Detect and set resolution
		void GCM_Renderer::InitDisplay() {
			CellVideoOutState videoState;
			CellVideoOutResolution resolution;

			//Get the current display mode, 
			// This has to have been previously set in the target manager at some point
			int err = cellVideoOutGetState(CELL_VIDEO_OUT_PRIMARY, 0, &videoState);
			DBG_ASSERT_MSG(err == CELL_OK, "cellVideoOutGetState failed !");

			err = cellVideoOutGetResolution(videoState.displayMode.resolutionId, &resolution);
			DBG_ASSERT_MSG(err == CELL_OK, "cellVideoOutGetResolution failed !");

			printf("Output Resolution:\t%i x %i \n", resolution.width, resolution.height);
			screenWidth = resolution.width;
			screenHeight = resolution.height;

			//Rebuild a CellVideoOutConfiguration, using the current resolution
			uint32_t color_depth = 4; // ARGB8
			uint32_t z_depth = 4;     // COMPONENT24
			color_pitch = resolution.width*color_depth;
			color_size = color_pitch * resolution.height;
			depth_pitch = resolution.width*z_depth;
			depthSize = depth_pitch * resolution.height;

			CellVideoOutConfiguration video_cfg;
			//Fill videocfg with 0
			memset(&video_cfg, 0, sizeof(CellVideoOutConfiguration));

			video_cfg.resolutionId = videoState.displayMode.resolutionId;
			video_cfg.format = CELL_VIDEO_OUT_BUFFER_COLOR_FORMAT_X8R8G8B8;
			video_cfg.pitch = color_pitch;

			//Set the video configuration, we haven't changed anything other than possibly the Z/colour depth
			err = cellVideoOutConfigure(CELL_VIDEO_OUT_PRIMARY, &video_cfg, NULL, 0);
			DBG_ASSERT_MSG(err == CELL_OK, "cellVideoOutConfigure failed !");

			//Fetch videoState again, just to make sure everything went ok
			err = cellVideoOutGetState(CELL_VIDEO_OUT_PRIMARY, 0, &videoState);
			DBG_ASSERT_MSG(err == CELL_OK, "cellVideoOutGetState failed !");

			//Store the aspect ratio
			switch (videoState.displayMode.aspect){
			case CELL_VIDEO_OUT_ASPECT_4_3:
				screenRatio = 4.0f / 3.0f;
				break;
			case CELL_VIDEO_OUT_ASPECT_16_9:
				screenRatio = 16.0f / 9.0f;
				break;
			default:
				printf("unknown aspect ratio %x\n", videoState.displayMode.aspect);
				screenRatio = 16.0f / 9.0f;
			}

			cellGcmSetFlipMode(CELL_GCM_DISPLAY_VSYNC);
			projMatrix = M4::perspective(60.0f, (16.0f / 9.0f), 1.0f, 2000.0f);
		}

		// Create Buffers/surfaces in memory
		void GCM_Renderer::InitSurfaces()
		{
			printf("Creating buffers\n");

			//GCMconfig holds info regarding memory and clock speeds
			CellGcmConfig config;
			cellGcmGetConfiguration(&config);

			//Get the base address of the mapped RSX local memory 
			GCM_MemoryUtils::localHeapStart = (uint32_t)config.localAddress;

			//Allocate a 64byte aligned segment of RSX memory that is the size of a depth buffer
			void * depthBuffer = GCM_MemoryUtils::localMemoryAlign(64, depthSize);
			uint32_t depthOffset;

			/* cellGcmAddressToOffset converts an effective address in the area accessible by the RSX to an offset value.
				An offset is the space between from the base address of local memory and a certain useable address.
				Offsets are used in gcm commands that deal with shader parameters, texture mapping and vertex arrays.
				They serve no real use other than as a parameter for these functions.
				*/

			//The offset value will be stored into depthOffset.
			cellGcmAddressToOffset(depthBuffer, &depthOffset);

			for (int i = 0; i < _numberOfSurfaces; ++i)
			{
				///Allocate a 64byte aligned segment of RSX memory that is the size of a colour buffer
				void *buffer = GCM_MemoryUtils::localMemoryAlign(64, color_size);

				//Get the offset address for it and store it in surfaces[i].colorOffset [0]		
				cellGcmAddressToOffset(buffer, &_surfaces[i].colorOffset[0]);

				/* This function registers a buffer that outputs to a display.
				This is the point where the buffer is actually written to local memory.
				Parameters:
				cellGcmSetDisplayBuffer ( Buffer ID (0 - 7), memory offset, pitch - Horizontal byte width,
				width - Horizontal resolution (number of pixels), height - Vertical resolution(number of pixels)
				*/
				cellGcmSetDisplayBuffer(i, _surfaces[i].colorOffset[0], color_pitch, screenWidth, screenHeight);

				// Now we set other parameters on each CellGcmSurface object

				//whether to place the color buffer, main memory or local memory.
				_surfaces[i].colorLocation[0] = CELL_GCM_LOCATION_LOCAL;
				//Pitch size of the color buffer (resolution.width*color_depth)
				_surfaces[i].colorPitch[0] = color_pitch;
				//Target of the color buffer    
				_surfaces[i].colorTarget = CELL_GCM_SURFACE_TARGET_0;

				//Init the color buffers
				//Up to 4 color buffers can be used on a CellGcmSurface, but we only use 1.
				for (int j = 1; j < 4; ++j)
				{
					_surfaces[i].colorLocation[j] = CELL_GCM_LOCATION_LOCAL;
					_surfaces[i].colorOffset[j] = 0;
					_surfaces[i].colorPitch[j] = 64;
				}

				//Type of render target (Pitch or swizzle)
				_surfaces[i].type = CELL_GCM_SURFACE_PITCH;
				//Antialiasing format type (None in this case)
				_surfaces[i].antialias = CELL_GCM_SURFACE_CENTER_1;
				//Format of the color buffer
				_surfaces[i].colorFormat = CELL_GCM_SURFACE_A8R8G8B8;
				//Format of the depth and stencil buffers (16-bit depth or 24-bit depth and 8-bit stencil)
				_surfaces[i].depthFormat = CELL_GCM_SURFACE_Z24S8;
				//whether to place the depth buffer, main memory or local memory.
				_surfaces[i].depthLocation = CELL_GCM_LOCATION_LOCAL;
				//The offset address to our depth buffer (We only need 1 for both surfaces)
				_surfaces[i].depthOffset = depthOffset;
				//Pitch size of the depth buffer (resolution.width*z_depth)
				_surfaces[i].depthPitch = depth_pitch;
				//Dimensions (in pixels)
				_surfaces[i].width = screenWidth;
				_surfaces[i].height = screenHeight;
				//Window offsets
				_surfaces[i].x = 0;
				_surfaces[i].y = 0;
			}


			/*
			The surfaces[] array contains CellGcmSurface objects and is in stack memory somewhere,
			and a bunch of new buffer objects have just been created and stored in RSX Local Memory.
			Each CellGcmSurface object has a pointer to its corresponding buffer in .colorOffset [0].
			When we call cellGcmSetSurface(), we pass it an CellGcmSurface from our array,
			The parameters that we set on that object will be read, processed and passed to the RSX.
			*/

			//Set Surface[0] to be the first surface to render to
			cellGcmSetSurface(&_surfaces[0]);
			//Used to keep track of the surface currently being rendered to.
			_swapFlag = false;
		}

		void GCM_Renderer::setupFrame() {

			cellGcmSetColorMask(CELL_GCM_COLOR_MASK_R |
				CELL_GCM_COLOR_MASK_G |
				CELL_GCM_COLOR_MASK_B |
				CELL_GCM_COLOR_MASK_A);

			cellGcmSetDepthTestEnable(CELL_GCM_TRUE);
			//cellGcmSetDepthTestEnable ( CELL_GCM_FALSE );

			cellGcmSetDepthFunc(CELL_GCM_LESS);
			//cellGcmSetDepthFunc(CELL_GCM_NEVER);

			cellGcmSetCullFaceEnable(CELL_GCM_FALSE);
			//cellGcmSetBlendEnable(CELL_GCM_FALSE);

			cellGcmSetShadeMode(CELL_GCM_SMOOTH);

			cellGcmSetBlendEnable(CELL_GCM_FALSE);

			// set polygon fill mode
			cellGcmSetFrontPolygonMode(CELL_GCM_POLYGON_MODE_FILL);

			//check for events
			//TODO: move this somewhere better
			cellSysutilCheckCallback();


		}

		void GCM_Renderer::clearSurface() {

			cellGcmSetClearColor((64 << 2) | (64 << 2) | (64 << 2) | (255 << 24));
			cellGcmSetClearSurface(CELL_GCM_CLEAR_Z | CELL_GCM_CLEAR_S | CELL_GCM_CLEAR_R | CELL_GCM_CLEAR_G | CELL_GCM_CLEAR_B | CELL_GCM_CLEAR_A);

		}

		// Switch which buffer is active(being rendered on) and currentyl displayed
		void GCM_Renderer::swapBuffers()
		{
			//non-zero indicates hardware is still  processing the last flip
			while (cellGcmGetFlipStatus() != 0)
			{
				sys_timer_usleep(300);
			}

			//reset flips status
			cellGcmResetFlipStatus();

			//flip
			cellGcmSetFlip((uint8_t)_swapFlag);

			//flush the pipline
			cellGcmFlush();
			cellGcmSetWaitFlip();

			//Toggle the swapvalue flag
			_swapFlag = !_swapFlag;

			//Tell gcm to render into the correct surface
			//TODO: make this suitable for more than 2 surfaces.
			cellGcmSetSurface(&_surfaces[_swapFlag]);
		}

		// Set the active vertex and fragment shader
		void GCM_Renderer::SetCurrentShader(GCM_VertexShader & vert, GCM_FragmentShader & frag)
		{
			_currentFrag = &frag;
			_currentVert = &vert;

			cellGcmSetFragmentProgram(_currentFrag->program, _currentFrag->offset);
			cellGcmSetVertexProgram(_currentVert->program, _currentVert->ucode);
		}


		// Initialises viewport (coordinate scaling)
		void GCM_Renderer::SetViewport() {
			uint16_t x, y, w, h;
			float min, max;
			float scale[4], offset[4];

			x = 0; // starting position of the viewport ( left of screen )
			y = 0; // starting position of the viewport (top of screen )
			w = screenWidth; // Width of viewport
			h = screenHeight; // Height of viewport
			min = 0.0f; // Minimum z value
			max = 1.0f; // Maximum z value

			// Scale our NDC coordinates to the size of the screen
			scale[0] = w * 0.5f;
			scale[1] = h * -0.5f; // Flip y axis !
			scale[2] = (max - min) * 0.5f;
			scale[3] = 0.0f;

			// Translate from a range starting from -1 to a range starting at 0
			offset[0] = x + scale[0];
			offset[1] = y + h * 0.5f;
			offset[2] = (max + min) * 0.5f;
			offset[3] = 0.0f;

			// analogous to the glViewport function ... but with extra values !
			cellGcmSetViewport(x, y, w, h, min, max, scale, offset);
		}

		void  GCM_Renderer::loadDefaultShaders()
		{
			puts("Making Default Shaders\n");

			defaultFrag = new GCM_FragmentShader();
			//FS->loadCharArrayShader(fs_basic);
			defaultFrag->LoadBinaryShader("/fs_basic.fpo");
			defaultFrag->initProgram(true);

			defaultVert = new GCM_VertexShader();
			//VS->loadCharArrayShader(vs_basic);
			defaultVert->LoadBinaryShader("/vs_basic.vpo");
			defaultVert->initProgram(false);

			puts("Shaders parsed\n");

			defaultVert->SetDefaultAttributes();

			// Set current shader
			SetCurrentShader(*defaultVert, *defaultFrag);

		}

		void GCM_Renderer::renderMesh(stMesh* msh, matrix4 mvp)
		{
			DBG_ASSERT(msh->loadedLocal);
			//set active shader
			GCM_Renderer::SetCurrentShader(*msh->vertShader, *msh->fragShader);

			//give vertex data to the shader
			cell::Gcm::cellGcmSetVertexDataArray(msh->vertShader->VERTEX_POSITION_INDEX,	//index
				0, //Frequency
				sizeof(stVertex), //stride
				3, //size
				CELL_GCM_VERTEX_F, //type
				CELL_GCM_LOCATION_LOCAL, //location
				msh->vertexBufferOffset);	//offset

			cell::Gcm::cellGcmSetVertexDataArray(msh->vertShader->VERTEX_COLOUR_INDEX,
				0,
				sizeof(stVertex),
				4,
				CELL_GCM_VERTEX_UB,
				CELL_GCM_LOCATION_LOCAL,
				msh->vertexBufferOffset + sizeof(float) * 3);

			msh->vertShader->SetParameterM("modelViewProj", mvp);

			//not sure wether to call this?
			//FS->UpdateShaderVariables();

			//draw arrays
			if (msh->strip == true){

			}else{

			}
			if (msh->line == true){
				if (msh->strip == true){
					cell::Gcm::cellGcmSetDrawArrays(CELL_GCM_PRIMITIVE_LINE_STRIP, 0, msh->numVerts);
				}
				else{
					cell::Gcm::cellGcmSetDrawArrays(CELL_GCM_PRIMITIVE_LINES, 0, msh->numVerts);
				}
			}
			else{
				if (msh->fan == true){
					cell::Gcm::cellGcmSetDrawArrays(CELL_GCM_PRIMITIVE_TRIANGLE_FAN, 0, msh->numVerts);
				}
				else if (msh->strip == true){
					cell::Gcm::cellGcmSetDrawArrays(CELL_GCM_PRIMITIVE_TRIANGLE_STRIP, 0, msh->numVerts);
				}
				else{
					cell::Gcm::cellGcmSetDrawArrays(CELL_GCM_PRIMITIVE_TRIANGLES, 0, msh->numVerts);
				}
			}


		}



		// ******************************************************************************



		/*
		void main
		(
		float4 color_in      : COLOR,
		out float4 color_out : COLOR
		)
		{
		color_out = color_in;
		}
		*/
		unsigned char fs_basic[208] = {
			0x00, 0x00, 0x1B, 0x5C, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0xD0, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x20,
			0x00, 0x00, 0x00, 0xA0, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0xC0, 0x00, 0x00, 0x04, 0x18, 0x00, 0x00, 0x0A, 0xC5,
			0x00, 0x00, 0x10, 0x05, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x10, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x04, 0x18, 0x00, 0x00, 0x0A, 0xC5, 0x00, 0x00, 0x10, 0x05, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x95,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8F, 0x00, 0x00, 0x10, 0x02, 0x00, 0x00, 0x00, 0x01,
			0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x43, 0x4F, 0x4C, 0x4F, 0x52, 0x00, 0x63, 0x6F, 0x6C, 0x6F, 0x72, 0x5F,
			0x69, 0x6E, 0x00, 0x43, 0x4F, 0x4C, 0x4F, 0x52, 0x00, 0x63, 0x6F, 0x6C, 0x6F, 0x72, 0x5F, 0x6F, 0x75, 0x74, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x02, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x01, 0x01, 0x00, 0xC8, 0x01, 0x1C, 0x9D,
			0xC8, 0x00, 0x00, 0x01, 0xC8, 0x00, 0x3F, 0xE1
		};

		/*
		void main
		(
		float4 position	: POSITION,
		float4 color	: COLOR,

		uniform float4x4 modelViewProj,

		out float4 oPosition : POSITION,
		out float4 oColor    : COLOR
		)
		{
		oPosition = mul(ModelViewProjMatrix, position);
		oColor = color;
		}
		*/
		unsigned char vs_basic[672] = {
			0x00, 0x00, 0x1B, 0x5B, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x02, 0xA0, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x20,
			0x00, 0x00, 0x02, 0x60, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x02, 0x80, 0x00, 0x00, 0x04, 0x18, 0x00, 0x00, 0x08, 0x41,
			0x00, 0x00, 0x10, 0x05, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x01, 0xD9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x01, 0xD0, 0x00, 0x00, 0x10, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x04, 0x18, 0x00, 0x00, 0x08, 0x44, 0x00, 0x00, 0x10, 0x05, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x01, 0xE8,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xE2, 0x00, 0x00, 0x10, 0x01, 0x00, 0x00, 0x00, 0x01,
			0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x28, 0x00, 0x00, 0x0C, 0xB8, 0x00, 0x00, 0x10, 0x06,
			0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x01, 0xEE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x10, 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x18,
			0x00, 0x00, 0x0C, 0xB8, 0x00, 0x00, 0x10, 0x06, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x01, 0xFC, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x18, 0x00, 0x00, 0x0C, 0xB8, 0x00, 0x00, 0x10, 0x06, 0xFF, 0xFF, 0xFF, 0xFF,
			0x00, 0x00, 0x02, 0x0D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x01,
			0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x18, 0x00, 0x00, 0x0C, 0xB8,
			0x00, 0x00, 0x10, 0x06, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x02, 0x1E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x04, 0x18, 0x00, 0x00, 0x0C, 0xB8, 0x00, 0x00, 0x10, 0x06, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x02, 0x2F,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x01, 0x00, 0x00, 0x00, 0x02,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x18, 0x00, 0x00, 0x08, 0xC3, 0x00, 0x00, 0x10, 0x05,
			0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x02, 0x49, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x40,
			0x00, 0x00, 0x10, 0x02, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x18,
			0x00, 0x00, 0x08, 0xC5, 0x00, 0x00, 0x10, 0x05, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x02, 0x59, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x53, 0x00, 0x00, 0x10, 0x02, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x01,
			0x00, 0x00, 0x00, 0x00, 0x50, 0x4F, 0x53, 0x49, 0x54, 0x49, 0x4F, 0x4E, 0x00, 0x70, 0x6F, 0x73, 0x69, 0x74, 0x69, 0x6F,
			0x6E, 0x00, 0x43, 0x4F, 0x4C, 0x4F, 0x52, 0x00, 0x63, 0x6F, 0x6C, 0x6F, 0x72, 0x00, 0x6D, 0x6F, 0x64, 0x65, 0x6C, 0x56,
			0x69, 0x65, 0x77, 0x50, 0x72, 0x6F, 0x6A, 0x00, 0x6D, 0x6F, 0x64, 0x65, 0x6C, 0x56, 0x69, 0x65, 0x77, 0x50, 0x72, 0x6F,
			0x6A, 0x5B, 0x30, 0x5D, 0x00, 0x6D, 0x6F, 0x64, 0x65, 0x6C, 0x56, 0x69, 0x65, 0x77, 0x50, 0x72, 0x6F, 0x6A, 0x5B, 0x31,
			0x5D, 0x00, 0x6D, 0x6F, 0x64, 0x65, 0x6C, 0x56, 0x69, 0x65, 0x77, 0x50, 0x72, 0x6F, 0x6A, 0x5B, 0x32, 0x5D, 0x00, 0x6D,
			0x6F, 0x64, 0x65, 0x6C, 0x56, 0x69, 0x65, 0x77, 0x50, 0x72, 0x6F, 0x6A, 0x5B, 0x33, 0x5D, 0x00, 0x50, 0x4F, 0x53, 0x49,
			0x54, 0x49, 0x4F, 0x4E, 0x00, 0x6F, 0x50, 0x6F, 0x73, 0x69, 0x74, 0x69, 0x6F, 0x6E, 0x00, 0x43, 0x4F, 0x4C, 0x4F, 0x52,
			0x00, 0x6F, 0x43, 0x6F, 0x6C, 0x6F, 0x72, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
			0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x40, 0x1F, 0x9C, 0x6C, 0x00, 0x40, 0x00, 0x0D, 0x81, 0x06, 0xC0, 0x83, 0x60, 0x41, 0xFF, 0x80, 0x40, 0x1F, 0x9C, 0x6C,
			0x00, 0x40, 0x03, 0x0D, 0x81, 0x06, 0xC0, 0x83, 0x60, 0x41, 0xFF, 0x85
		};
	}
}