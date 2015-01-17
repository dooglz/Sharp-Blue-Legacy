#include "OGL_Renderer.h"
#include "SDL_platform.h"
#include "../Maths.h"
#include "../Mesh.h"
#include "sdl/SDL.h"
#include "glew/glew.h"
#include <glm/gtc/type_ptr.hpp>
#include "OGL_ShaderProgram.h"
#include "OGL_FragmentShader.h"
#include "OGL_VertexShader.h"

namespace Engine {
namespace OGL {
float f = 0.0f;

OGL::OGL_ShaderProgram* COGL_Renderer::_defaultProgram;
std::vector<const Vector3> COGL_Renderer::linebuffer;

void COGL_Renderer::Init() { 
    loadShaders(); 
}

void COGL_Renderer::ClearSurface() {
  // f += 0.001f;
  // glClearColor(sin(f) , 0.5f, 0.5f, 1.f);
  glClearColor(1.f, 1.f, 1.f, 1.f);
  SDL::SDL_Platform::CheckGL();
  // SDL_Delay( 16 );

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  SDL::SDL_Platform::CheckGL();
}

COGL_Renderer::COGL_Renderer() {}

void COGL_Renderer::RenderMesh(Mesh* const msh, const Matrix4& mvp) {

  Matrix4 m4 = _viewprojectionMat * mvp;

  unsigned int programID;
  // ASSERT(msh->program != NULL);
  if (msh->program == NULL){
      //TODO: throw warning
      programID = GetDefaultShaderProgram()->getID();
  }
  else{
      programID = msh->program->getID();
  }

  glUseProgram(programID);
  SDL::SDL_Platform::CheckGL();

  GLint mvpIn = glGetUniformLocation(programID, "modelprojection");
  SDL::SDL_Platform::CheckGL();
  // Send VP
  glUniformMatrix4fv(mvpIn, 1, false, glm::value_ptr(mvp));
  SDL::SDL_Platform::CheckGL();

  // Bind to VAO
  glBindVertexArray(msh->gVAO);
  SDL::SDL_Platform::CheckGL();

  // DRAW
  if (msh->line == true) {
    if (msh->strip == true) {
      glDrawArrays(GL_LINE_STRIP, 0, msh->numVerts);
    } else {
      glDrawArrays(GL_LINES, 0, msh->numVerts);
    }
  } else {
    if (msh->fan == true) {
      glDrawArrays(GL_TRIANGLE_FAN, 0, msh->numVerts);
    } else if (msh->strip == true) {
      glDrawArrays(GL_TRIANGLE_STRIP, 0, msh->numVerts);
    } else {
      glDrawArrays(GL_TRIANGLES, 0, msh->numVerts);
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
  //	glEnable(GL_CULL_FACE);
  // glViewport(0,0,1920,1080);

  SDL::SDL_Platform::CheckGL();
  ClearSurface();

  // TODO - change this to _current_program;
  glUseProgram(_defaultProgram->getID());
  SDL::SDL_Platform::CheckGL();
  GLint mvpIn =
      glGetUniformLocation(_defaultProgram->getID(), "viewprojection");
  SDL::SDL_Platform::CheckGL();
  // Send VP, technically we should only need to do this once. //TODO that
  glUniformMatrix4fv(mvpIn, 1, false, glm::value_ptr(_viewprojectionMat));
  SDL::SDL_Platform::CheckGL();
}

void COGL_Renderer::PostRender() {
  // Draw lines
  ProcessLines();
  SDL_GL_SwapWindow(SDL::SDL_Platform::GetWindow());
}

void COGL_Renderer::loadShaders() {
  // load Default Shaders
  printf("Loading Shaders \n");
  _defaultProgram = new OGL::OGL_ShaderProgram();

  // Create vertex shader
  OGL::OGL_VertexShader* VS = new OGL::OGL_VertexShader();
  VS->LoadSourceShader("shaders/basic.vert");
  SDL::SDL_Platform::CheckGL();

  // Attach vertex shader to program
  _defaultProgram->attachShader(VS);

  // Create Fragment shader
  OGL::OGL_FragmentShader* FS = new OGL::OGL_FragmentShader();
  FS->LoadSourceShader("shaders/basic.frag");
  SDL::SDL_Platform::CheckGL();

  // Attach vertex shader to program
  _defaultProgram->attachShader(FS);
  SDL::SDL_Platform::CheckGL();

  // Link program
  _defaultProgram->link();
}

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

  GLuint prgrm = GetDefaultShaderProgram()->getID();
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
    //(*it)->Update(delta);
  }

  glDeleteBuffers(1, &vbo);
  glDeleteVertexArrays(1, &vao);
  glBindVertexArray(NULL);
  glEnableVertexAttribArray(NULL);
  SDL::SDL_Platform::CheckGL();
  linebuffer.clear();
}

OGL::OGL_ShaderProgram* COGL_Renderer::GetDefaultShaderProgram() {
  if (_defaultProgram == nullptr) {
    loadShaders();
  }
  return _defaultProgram;
}
}
}