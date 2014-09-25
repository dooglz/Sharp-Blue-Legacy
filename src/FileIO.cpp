#include "FileIO.h"
#include "Utilities.h"
#include <iostream>
#include <fstream>
//disable MS specifc safe warnings
namespace Engine{

	char * FileIO::LoadTextFile(const std::string name)
	{
		// Look for file
		std::ifstream file(name.c_str(), std::ios::in | std::ios::binary);
		ASSERT_FUNC((file), printf("Can 't find file: %s\n", name.c_str()));

		// Create a buffer for holding the lines of the file
		// With a max line length of 256 characters
		char line[256];
		//String to hold the total file contents.
		std::string stringbuffer;
		// char* version of stringbuffer as the return value
		char *ret_val = NULL;

		//Read the file In
		if (file.is_open())
		{
			while (file.good())
			{
				// Get the line from file
				file.getline(line, 256);
				// Append to stringbuffer
				stringbuffer.append(line);
				stringbuffer.append("\n");
			}
			//Copy string buffer into a Char*
			ret_val = new char[stringbuffer.length() + 1];
			strncpy(ret_val, stringbuffer.c_str(), stringbuffer.length() + 1);
		}

		return ret_val;
	}

	char * FileIO::LoadBinaryFile(const std::string name)
	{
		// Look for file
		std::ifstream file(name.c_str(), std::ios::in | std::ios::binary);
		ASSERT_FUNC((file), printf("Can 't find file: %s\n", name.c_str()));

		// Load file attributes
		file.seekg(0, std::ios::end);
		unsigned int dataSize = (unsigned int)file.tellg();
		file.seekg(0, std::ios::beg);

		// Reserve memory
		char * data = (char *)malloc(dataSize);
		// Copy file into memory
		file.read(data, dataSize);
		file.close(); // Done with the data , close the file.

		return data;
	}
}