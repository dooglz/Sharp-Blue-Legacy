#include "Cm_Camera.h"
#include "Scene.h"

namespace Engine {
    namespace Components {
    CmCamera::CmCamera() : CComponent("Camera") {}

    CmCamera::~CmCamera() {}

    void CmCamera::Activate() {
        _active = true;
        ActiveScene->SetActiveCamera(this);
    }

    void CmCamera::Deactivate() {
        _active = false;
        ActiveScene->SetActiveCamera(NULL);
    }

    bool CmCamera::IsActive() { return _active; }
}
}