#include "GCM_Shader.h"
#include "GCM_Renderer.h"
#include <iostream>
#include <fstream>		// std::ifstream
#include <string>	// for memcpy(..)
#include <stdio.h>
#include <stdlib.h>
#include <sys/paths.h>	//includes SYS_APP_HOME
#include "GCM_MemoryUtils.h"
#include "FileIO.h"
namespace Engine{
	namespace GCM{

		// Parses a shader stored in a char array and stores it into Program in an unitialised state
		void GCM_Shader::loadCharArrayShader(const unsigned char * shaderCode)
		{
			program = (CGprogram)(void *)shaderCode;
		}

		// Loads a compiled shader from a file and stores it into Program in an unitialised state
		void GCM_Shader::LoadBinaryShader(std::string name)
		{
			//attempt to Load File
			name = SYS_APP_HOME + name;
			std::ifstream file(name.c_str(), std::ios::in | std::ios::binary);
			char * data = FileIO::LoadBinaryFile(name);
			program = (CGprogram)(void *)data;
		}

		// Initializes the shader program.
		void GCM_Shader::initProgram(bool storeOnRSX)
		{
			//Initialize the Cg binary program on memory for use by RSX.
			cellGcmCgInitProgram(program);

			unsigned int ucodeSize;

			//Stores pointer to the microcode in the specified Cg binary program in *pUCode, 
			// and the size of the microcode into *pUCodeSize, 
			cellGcmCgGetUCode(program, &ucode, &ucodeSize);

			if (storeOnRSX)
			{
				//Reserve some local memory to store the shader microcode
				void * RSXfragAddr = GCM_MemoryUtils::localMemoryAlign(64, ucodeSize);

				//Copy the microcode into RSX local memory 
				memcpy(RSXfragAddr, ucode, ucodeSize);

				//convert an effective address in the area accessible by the RSX to an offset value.
				//If conversion is successful, the offset value will be stored in &offset.
				cellGcmAddressToOffset(RSXfragAddr, &offset);
			}

			printf("Shader loaded\t Size: %i bytes\n", ucodeSize);
		}

		// Get a shader parameter by name
		CGparameter GCM_Shader::GetParameter(std::string name)
		{

			std::map <std::string, CGparameter >::iterator i = _uniforms.find(name);

			if (i != _uniforms.end())
			{
				// if its in the map , return it!
				return i->second;
			}

			CGparameter p = cellGcmCgGetNamedParameter(program, name.c_str());

			if (!p)
			{
				std::cout << "Can 't find named parameter :" << name << std::endl;
			}

			_uniforms.insert(std::make_pair(name, p));
			return p;

		}

		// Transposes a matrix into a 2d float[] and stores into a named paramter value 
		void GCM_Shader::SetParameterM(std::string name, matrix4 & totranpose)
		{
			matrix4 tempMatrix = transpose(totranpose);
			SetParameter(name, (float *)& tempMatrix);
		}

		// Transposes a matrix into a 2d float[] and stores into a named paramter value 
		void GCM_Shader::SetParameterM(CGparameter param, matrix4 & totranpose)
		{
			matrix4 tempMatrix = transpose(totranpose);
			SetParameter(param, (float *)& tempMatrix);
		}

		// Set a named paramter value
		void GCM_Shader::SetParameter(std::string name, float * data)
		{
			CGparameter p = GetParameter(name);
			if (p)
			{
				SetParameter(p, data);
			}
		}
	}
}
