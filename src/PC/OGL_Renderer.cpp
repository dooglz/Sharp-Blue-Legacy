#include "OGL_Renderer.h"
#include "SDL_platform.h"
#include "OGL_Resources.h"
#include "../Storage.h"
#include "../Maths.h"
#include "../Resource.h"
#include "sdl/SDL.h"
#include "glew/glew.h"
#include <glm/gtc/type_ptr.hpp>
#include "OGL_ShaderProgram.h"
#include "OGL_FragmentShader.h"
#include "OGL_VertexShader.h"


namespace Engine {
namespace OGL {
float f = 0.0f;

// OGL::OGL_ShaderProgram* COGL_Renderer::_defaultProgram;
std::vector<const Vector3> COGL_Renderer::linebuffer;

void COGL_Renderer::Init() { 
loadShaders();
}

static float count = 0;
static bool colourCycle = true;

void COGL_Renderer::SetClearColour(const float r, const float g, const float b)
{
  colourCycle = false;
  glClearColor(r / 255.0f, g / 255.0f, b / 255.0f, 1.f);
  SDL::SDL_Platform::CheckGL();

}

void COGL_Renderer::ClearSurface() {
  if (colourCycle){
    count += 0.05f;
    float r = (sin((0.1f * count) + 0) * 127.0f) + 50.0f;
    float g = (sin((0.1f * count) + 2) * 127.0f) + 50.0f;
    float b = (sin((0.1f * count) + 4) * 127.0f) + 50.0f;
    glClearColor(r / 255.0f, g / 255.0f, b / 255.0f, 1.f);
    SDL::SDL_Platform::CheckGL();
  }

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  SDL::SDL_Platform::CheckGL();
}

COGL_Renderer::COGL_Renderer() {}

void COGL_Renderer::RenderMesh(RenderObject *const ro, const Matrix4 &mvp) {

  unsigned int programID;
  if (ro->material == NULL || ro->material->EngineMaterial == NULL) {
    ASSERT(false);
    return;
  }
  Material *material = ro->material;
  OGL_ShaderProgram *sp =
      static_cast<OGL_ShaderProgram *>(material->EngineMaterial);
  programID = sp->getID();

  glUseProgram(programID);
  SDL::SDL_Platform::CheckGL();

  // TODO: only do this once per frame per SP.
  GLint vpIn = glGetUniformLocation(programID, "viewprojection");
  SDL::SDL_Platform::CheckGL();
  glUniformMatrix4fv(vpIn, 1, false, glm::value_ptr(_viewprojectionMat));
  SDL::SDL_Platform::CheckGL();

  GLint mvpIn = glGetUniformLocation(programID, "modelprojection");
  SDL::SDL_Platform::CheckGL();
  // Does the shader have this input?
  ASSERT(mvpIn != -1);
  glUniformMatrix4fv(mvpIn, 1, false, glm::value_ptr(mvp));
  SDL::SDL_Platform::CheckGL();

  for (unsigned int i = 0; i < material->TexturesCount; i++) {
    // Bind toTexture Unit
    glActiveTexture(GL_TEXTURE0 + i);
    // bind texture to texture unit
    OGL_Texture *oglTex =
        static_cast<OGL_Texture *>(ro->textures[i]->EngineTexture);
    glBindTexture(GL_TEXTURE_2D, oglTex->TextureID);
    // Set Shader Texture sampler to Texture Unit
    // Texture uniforms starts at 50.
    glUniform1i(50 + i, 0 + i);
  }

  for (unsigned int i = 0; i < material->ParametersCount; i++) {
    // TODO
  }

  // Bind to VAO
  glBindVertexArray(ro->mesh->gVAO);
  SDL::SDL_Platform::CheckGL();

  // DRAW
  if (ro->mesh->line == true) {
    if (ro->mesh->strip == true) {
      glDrawArrays(GL_LINE_STRIP, 0, ro->mesh->numVerts);
    } else {
      glDrawArrays(GL_LINES, 0, ro->mesh->numVerts);
    }
  } else {
    if (ro->mesh->fan == true) {
      glDrawArrays(GL_TRIANGLE_FAN, 0, ro->mesh->numVerts);
    } else if (ro->mesh->strip == true) {
      glDrawArrays(GL_TRIANGLE_STRIP, 0, ro->mesh->numVerts);
    } else {
      glDrawArrays(GL_TRIANGLES, 0, ro->mesh->numVerts);
    }
  }
  SDL::SDL_Platform::CheckGL();

  // unbind VAO
  glBindVertexArray(0);
  SDL::SDL_Platform::CheckGL();

  // Unbind program
  glUseProgram(NULL);
  SDL::SDL_Platform::CheckGL();
}

void COGL_Renderer::PrepFrame() {
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  glClearDepth(1.0f);

  glViewport(0, 0, SDL::SDL_Platform::GetSreenWidth(),
             SDL::SDL_Platform::GetSreenHeight());

  // Cull triangles which normal is not towards the camera
  	//glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
  // glViewport(0,0,1920,1080);

  SDL::SDL_Platform::CheckGL();
  ClearSurface();

  // TODO - change this to _current_program;
  //  glUseProgram(_defaultProgram->getID());
  SDL::SDL_Platform::CheckGL();
  // GLint mvpIn =glGetUniformLocation(_defaultProgram->getID(),
  // "viewprojection");
  SDL::SDL_Platform::CheckGL();
  // Send VP, technically we should only need to do this once. //TODO that
  // glUniformMatrix4fv(mvpIn, 1, false, glm::value_ptr(_viewprojectionMat));
  SDL::SDL_Platform::CheckGL();
}

void COGL_Renderer::PostRender() {
  // Draw lines
  ProcessLines();
  SDL_GL_SwapWindow(SDL::SDL_Platform::GetWindow());
}

void COGL_Renderer::loadShaders() {}

void COGL_Renderer::DrawCross(const Vector3& p1, const float size) {
  DrawLine(p1 + Vector3(size, 0, 0), p1 - Vector3(size, 0, 0));
  DrawLine(p1 + Vector3(0, size, 0), p1 - Vector3(0, size, 0));
  DrawLine(p1 + Vector3(0, 0, size), p1 - Vector3(0, 0, size));
}

void COGL_Renderer::DrawLine(const Vector3& p1, const Vector3& p2) {
  linebuffer.push_back(p1);
  linebuffer.push_back(p2);
}

void COGL_Renderer::ProcessLines() {
  if (linebuffer.size() < 1) {
    return;
  }
  // Generate VAO
  unsigned int vao;
  glGenVertexArrays(1, &vao);
  SDL::SDL_Platform::CheckGL();

  // Bind VAO
  glBindVertexArray(vao);
  SDL::SDL_Platform::CheckGL();

  // Generate VBO
  unsigned int vbo;
  glGenBuffers(1, &vbo);
  SDL::SDL_Platform::CheckGL();

  // Bind VBO
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  SDL::SDL_Platform::CheckGL();

  // put the data in it
  glBufferData(GL_ARRAY_BUFFER, linebuffer.size() * sizeof(Vector3),
               &linebuffer[0], GL_STATIC_DRAW);
  SDL::SDL_Platform::CheckGL();

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0,               // index
                        3,               // size
                        GL_FLOAT,        // type
                        GL_FALSE,        // normalised
                        sizeof(Vector3), // stride
                        NULL             // pointer/offset
                        );
  SDL::SDL_Platform::CheckGL();

  GLuint prgrm = Storage<OGL_ShaderProgram>::Get("basic-basic")->getID();
  glUseProgram(prgrm);
  SDL::SDL_Platform::CheckGL();

  // get shader input indexes
  GLint mvpIn = glGetUniformLocation(prgrm, "modelprojection");
  SDL::SDL_Platform::CheckGL();

  // Send MVP
  glUniformMatrix4fv(mvpIn, 1, false, glm::value_ptr(Matrix4(1.0f)));
  SDL::SDL_Platform::CheckGL();
  glDisable(GL_DEPTH_TEST);
  glDrawArrays(GL_LINES, 0, linebuffer.size());
  SDL::SDL_Platform::CheckGL();

  for (std::vector<Vector3>::iterator it = linebuffer.begin();
       it != linebuffer.end(); ++it) {
  }

  glDeleteBuffers(1, &vbo);
  glDeleteVertexArrays(1, &vao);
  glBindVertexArray(NULL);
  SDL::SDL_Platform::CheckGL();
  linebuffer.clear();
}

OGL::OGL_ShaderProgram *COGL_Renderer::GetDefaultShaderProgram() {
  return Storage<OGL_ShaderProgram>::Get("basic-basic");
}

void COGL_Renderer::Shutdown() {
  linebuffer.clear();
  // delete _defaultProgram;
}


float COGL_Renderer::GetDepthAt(const unsigned int& x, const unsigned int& y)
{
  GLfloat depth;
  glReadPixels(x, 720-y , 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
  return depth;
}

}
};
