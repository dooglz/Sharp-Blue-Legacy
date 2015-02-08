#include "Cm_Camera.h"
#include "Scene.h"
#include "Entity.h"

namespace Engine {
namespace Components {
CmCamera::CmCamera() : CComponent("Camera") {}

CmCamera::~CmCamera() { 
  printf("camera destructor\n");
  if (IsActive()){ Ent->GetScene()->SetActiveCamera(NULL); }
}

bool CmCamera::IsActive() { return Ent->GetScene()->GetActiveCamera() == this; }
}
}