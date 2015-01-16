#include "Actor.h"
#include "GameEngine.h"
#include "Renderer.h"

namespace Engine {
bool Actor::isVisible() { return _visible; }
void Actor::setVisibility(const bool vis) { _visible = vis; }

Matrix4 Actor::getModelProjection() {
  // calculate Model projection
  Matrix4 scl = Scale(scale);
  // Matrix4 rot = M4::rotation(rotation);
  Matrix4 rot = EulerToMatrix(rotation);
  Matrix4 trn = Translation(position);
  // return (scl * rot * trn);
  return (trn * rot * scl);
}
void Actor::Render() {
  Engine::Renderer->RenderMesh(
      Mesh, Engine::Renderer->projMatrix *
                Engine::Renderer->viewMatrix *
                getModelProjection());
}
}