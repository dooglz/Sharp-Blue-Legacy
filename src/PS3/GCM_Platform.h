#pragma once
#include "Base_Platform.h"
namespace Engine {
namespace GCM {
class GCM_Platform : public Platform {
protected:
  void loadDefaultShaders();

public:
  GCM_Platform();
  ~GCM_Platform();
  double GetTime(){
      return (((double)(sys_time_get_system_time())) * 0.000001);
  }
  void InitDisplay(const unsigned short width, const unsigned short height);
  void shutdown();
};
}
}