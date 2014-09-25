#pragma once
#include "Utilities.h"		// asserts and commmon defines
#include <string>
#include <vector>

namespace Engine{
	struct Input_axis{
		std::string name;
		unsigned char value;
	};

	struct Input_map{
		std::string name;
		std::vector<std::string> defaults;
		std::vector<std::string> current;
	};

	class Input
	{
	public:
		//--Input_data

		static std::vector<Input_axis>::iterator find(std::string name);
		static unsigned char getValue(std::string name);
		static unsigned char getValue(std::vector<Input_axis>::iterator itr);
		static void setValue(const std::string name, const unsigned char value);
		static void setValue(std::vector<Input_axis>::iterator itr, unsigned char value);
		static void removeAxis(std::string name);
		static void removeAxis(std::vector<Input_axis>::iterator itr);
		static void addAxis(std::string name);
		static bool axisExists(std::string name);

		//--Input mapping
		static std::vector<Input_map>::iterator findMap(std::string name);
		static void addMap(Input_map map);
		static void addMap(std::string name, std::vector<std::string> defaults);
		static void removeMap(std::string name);
		static void removeMap(std::vector<Input_map>::iterator itr);
		static unsigned char getMapData(std::string name);
		static bool mapExists(std::string name);

	protected:
		static std::vector<Input_axis> input_data;
		static std::vector<Input_map> input_mapping;
	};
}