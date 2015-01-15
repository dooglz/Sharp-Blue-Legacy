#include "Input.h"
namespace Engine {

std::vector<Input_axis> Input::input_data;
std::vector<Input_map> Input::input_mapping;

bool Input::axisExists(std::string name) {
  std::vector<Input_axis>::iterator itr;
  for (itr = input_data.begin(); itr != input_data.end(); ++itr) {
    if (itr->name == name) {
      return true;
    }
  }
  return false;
}

std::vector<Input_axis>::iterator Input::find(std::string name) {
  std::vector<Input_axis>::iterator itr;
  for (itr = input_data.begin(); itr != input_data.end(); ++itr) {
    if (itr->name == name) {
      return itr;
    }
  }
  return itr;
}

unsigned char Input::getValue(std::string name) {
  if (axisExists(name)) {
    return getValue(find(name));
  }
  return 0;
}
unsigned char Input::getValue(std::vector<Input_axis>::iterator itr) {
  return itr->value;
}

void Input::setValue(std::vector<Input_axis>::iterator itr,
                     unsigned char value) {
  itr->value = value;
}

void Input::setValue(const std::string name, const unsigned char value) {
  // printf("input! %s = %u\n", name.c_str(), value);
  if (axisExists(name)) {
    find(name)->value = value;
  }
}

void Input::removeAxis(std::string name) {
  if (axisExists(name)) {
    removeAxis(find(name));
  }
}

void Input::removeAxis(std::vector<Input_axis>::iterator itr) {
  input_data.erase(itr);
}

void Input::addAxis(std::string name) {
  Input_axis IA;
  IA.name = name;
  IA.value = 0;
  input_data.push_back(IA);
}

bool Input::mapExists(std::string name) {
  std::vector<Input_map>::iterator itr;
  for (itr = input_mapping.begin(); itr != input_mapping.end(); ++itr) {
    if (itr->name == name) {
      return true;
    }
  }
  return false;
}

std::vector<Input_map>::iterator Input::findMap(std::string name) {
  std::vector<Input_map>::iterator itr;
  for (itr = input_mapping.begin(); itr != input_mapping.end(); ++itr) {
    if (itr->name == name) {
      return itr;
    }
  }
  return itr;
}

void Input::addMap(Input_map map) { input_mapping.push_back(map); }

void Input::addMap(std::string name, std::vector<std::string> defaults) {
  Input_map m;
  m.name = name;
  m.defaults = defaults;
  m.current = defaults;
  input_mapping.push_back(m);
}

void Input::removeMap(std::string name) {
  if (mapExists(name)) {
    removeMap(findMap(name));
  }
}

void Input::removeMap(std::vector<Input_map>::iterator itr) {
  input_mapping.erase(itr);
}

unsigned char Input::getMapData(std::string name) {
  if (mapExists(name)) {
    std::vector<std::string> axises = findMap(name)->current;
    std::vector<std::string>::iterator itr;
    // foreach axis name in the map
    for (itr = axises.begin(); itr != axises.end(); ++itr) {
      // find axis
      if (axisExists(*itr)) {
        std::vector<Input_axis>::iterator itr2 = find(*itr);
        if (itr2 != input_data.end()) {
          // found! Is it active?
          unsigned char val = itr2->value;
          if (val > 0) {
            return val;
          }
        }
      }
    }
  }
  return 0;
}
}