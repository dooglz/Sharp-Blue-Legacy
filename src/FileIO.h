#pragma once
#include <string>
#pragma warning(disable:4496)
#pragma warning(disable:4996)

namespace Engine{
	class FileIO{

	protected:

	public:
		static char * LoadTextFile(const std::string name);
		static char * LoadBinaryFile(const std::string name);
	};
}