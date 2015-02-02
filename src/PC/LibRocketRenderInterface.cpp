#include "LibRocketRenderInterface.h"
#include "../Utilities.h"
#include <Rocket/Core.h>
#include "OGL_ShaderProgram.h"
#include "../Storage.h"
#include "glew/glew.h"
#include <glm/gtc/type_ptr.hpp>
#include "OGL_Resources.h"

namespace Engine {

CLibRocketRenderInterface::CLibRocketRenderInterface() {}


// Called by Rocket when it wants to render geometry that it does not wish to
// optimise.

void CLibRocketRenderInterface::RenderGeometry(
    Rocket::Core::Vertex* vertices, int num_vertices, int* indices,
    int num_indices, const Rocket::Core::TextureHandle texture,
    const Rocket::Core::Vector2f& translation) {

  GLuint VAO;

  // Generate VAO
  glGenVertexArrays(1, &VAO);
  SDL::SDL_Platform::CheckGL();
  //printf("Render %i\n", VAO);
  // Bind VAO
  glBindVertexArray(VAO);
  SDL::SDL_Platform::CheckGL();

  // Vertex
  // Generate VBO
  GLuint VBO;
  glGenBuffers(1, &VBO);
  SDL::SDL_Platform::CheckGL();

  // Bind VBO
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  SDL::SDL_Platform::CheckGL();

  // put the data in it
  glBufferData(GL_ARRAY_BUFFER, num_vertices * sizeof(Rocket::Core::Vertex),
               &vertices[0], GL_STATIC_DRAW);
  SDL::SDL_Platform::CheckGL();

  // Position
  glEnableVertexAttribArray(0);
  SDL::SDL_Platform::CheckGL();
  glVertexAttribPointer(0,                            // index
                        2,                            // size
                        GL_FLOAT,                     // type
                        GL_FALSE,                     // normalised
                        sizeof(Rocket::Core::Vertex), // stride
                        NULL                          // pointer/offset
                        );
  SDL::SDL_Platform::CheckGL();
  OGL::OGL_ShaderProgram* sp;
  if (texture == NULL) { // Just Colours
    sp = Storage<OGL::OGL_ShaderProgram>::Get("ui-ui");
    glUseProgram(sp->getID());
    SDL::SDL_Platform::CheckGL();

    glEnableVertexAttribArray(1);
    SDL::SDL_Platform::CheckGL();
    glVertexAttribPointer(1,                            // index
                          4,                            // size
                          GL_UNSIGNED_BYTE,             // type
                          GL_TRUE,                      // normalised
                          sizeof(Rocket::Core::Vertex), // stride
                          &vertices[0].colour           // pointer/offset
                          );

    SDL::SDL_Platform::CheckGL();

  } else { // Just Textures
    sp = Storage<OGL::OGL_ShaderProgram>::Get("uiTex-uiTex");
    glUseProgram(sp->getID());
    SDL::SDL_Platform::CheckGL();

    glEnableVertexAttribArray(2);
    SDL::SDL_Platform::CheckGL();
    glVertexAttribPointer(2,                            // index
                          2,                            // size
                          GL_FLOAT,                     // type
                          GL_FALSE,                     // normalised
                          sizeof(Rocket::Core::Vertex), // stride
                          &vertices[0].tex_coord        // pointer/offset
                          );

    SDL::SDL_Platform::CheckGL();


    // Bind toTexture Unit
    glActiveTexture(GL_TEXTURE0);
    SDL::SDL_Platform::CheckGL();

    // bind texture to texture unit
    glBindTexture(GL_TEXTURE_2D, (GLuint)texture);
    SDL::SDL_Platform::CheckGL();
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Rocket::Core::Vertex),
                      &vertices[0].tex_coord);
    SDL::SDL_Platform::CheckGL();

    GLint texIn = glGetUniformLocation(sp->getID(), "texture");
    glUniform1i(texIn, 0);
    SDL::SDL_Platform::CheckGL();
  }

  glUniform2f(0, translation.x, translation.y);
  SDL::SDL_Platform::CheckGL();
  glUniformMatrix4fv(1, 1, false,
                     glm::value_ptr(glm::ortho(0.0f, 1280.0f, 720.0f, 0.0f)));
  SDL::SDL_Platform::CheckGL();

  // Generate a buffer for the indices
  GLuint elementbuffer;
  glGenBuffers(1, &elementbuffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, num_indices * sizeof(int), &indices[0],
               GL_STATIC_DRAW);
  SDL::SDL_Platform::CheckGL();

  glDisable(GL_DEPTH_TEST);
  glDrawElements(GL_TRIANGLES,    // mode
                 num_indices,     // count
                 GL_UNSIGNED_INT, // type
                 (void*)0         // element array buffer offset
                 );
  SDL::SDL_Platform::CheckGL();

  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &elementbuffer);
  glDeleteVertexArrays(1, &VAO);
  SDL::SDL_Platform::CheckGL();
}

struct geoHandle {
  OGL::OGL_ShaderProgram* sp;
  GLuint VAO;
  GLsizei Indices;
  GLuint Texture;
  bool Textured;
};
// Called by Rocket when it wants to compile geometry it believes will be static
// for the forseeable future.
Rocket::Core::CompiledGeometryHandle CLibRocketRenderInterface::CompileGeometry(
    Rocket::Core::Vertex* vertices, int num_vertices, int* indices,
    int num_indices, const Rocket::Core::TextureHandle texture) {
  geoHandle* gh = new geoHandle();
  
  GLuint VAO;
  // Generate VAO
  glGenVertexArrays(1, &VAO);
  SDL::SDL_Platform::CheckGL();

  gh->VAO = VAO;
  gh->Indices = num_indices;
  //printf("Compiling %i\n", VAO);
  // Bind VAO
  glBindVertexArray(VAO);
  SDL::SDL_Platform::CheckGL();

  // Vertex
  // Generate VBO
  GLuint VBO;
  glGenBuffers(1, &VBO);
  SDL::SDL_Platform::CheckGL();

  // Bind VBO
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  SDL::SDL_Platform::CheckGL();

  // put the data in it
  glBufferData(GL_ARRAY_BUFFER, num_vertices * sizeof(Rocket::Core::Vertex),
               &vertices[0], GL_STATIC_DRAW);
  SDL::SDL_Platform::CheckGL();

  // Position
  glEnableVertexAttribArray(0);
  SDL::SDL_Platform::CheckGL();

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Rocket::Core::Vertex), NULL);
  SDL::SDL_Platform::CheckGL();

  if (texture == NULL) { // Just Colours
    gh->Textured = false;
    gh->sp = Storage<OGL::OGL_ShaderProgram>::Get("ui-ui");

    glEnableVertexAttribArray(1);
    SDL::SDL_Platform::CheckGL();

    glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE,
      sizeof(Rocket::Core::Vertex), (GLvoid*)(sizeof(Rocket::Core::Vector2f)));
    SDL::SDL_Platform::CheckGL();

  } else { // Just Textures
    gh->Textured = true;
    gh->sp = Storage<OGL::OGL_ShaderProgram>::Get("uiTex-uiTex");
    gh->Texture = texture;

    // Tell OGL how the UVS are stored
    glEnableVertexAttribArray(2);
    SDL::SDL_Platform::CheckGL();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, (GLuint)texture);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Rocket::Core::Vertex),
      (GLvoid*)(sizeof(Rocket::Core::Vector2f) + sizeof(Rocket::Core::Colourb)));
    SDL::SDL_Platform::CheckGL();
  }

  // Generate a buffer for the indices
  GLuint elementbuffer;
  glGenBuffers(1, &elementbuffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, num_indices * sizeof(int), &indices[0],
               GL_STATIC_DRAW);
  SDL::SDL_Platform::CheckGL();
  glBindVertexArray(0);
  return (Rocket::Core::CompiledGeometryHandle)gh;
}

// Called by Rocket when it wants to render application-compiled geometry.
void CLibRocketRenderInterface::RenderCompiledGeometry(
    Rocket::Core::CompiledGeometryHandle geometry,
    const Rocket::Core::Vector2f& translation) {

  geoHandle* gh = (geoHandle*)geometry;

 // printf("RenderCompiled %i\n", gh->VAO);
  glUseProgram(gh->sp->getID());

  // Bind VAO
  glBindVertexArray(gh->VAO);
  SDL::SDL_Platform::CheckGL();

  // transform uniform
  glUniform2f(0, translation.x, translation.y);
  SDL::SDL_Platform::CheckGL();

  // projection uniform
  glUniformMatrix4fv(1, 1, false,
                     glm::value_ptr(glm::ortho(0.0f, 1280.0f, 720.0f, 0.0f)));
  SDL::SDL_Platform::CheckGL();

  if (gh->Textured) {
    // Put the Texture in a texture unit
    // Bind toTexture Unit
   glActiveTexture(GL_TEXTURE0);
    SDL::SDL_Platform::CheckGL();
    // bind texture to texture unit
    glBindTexture(GL_TEXTURE_2D, gh->Texture);
    SDL::SDL_Platform::CheckGL();
    GLint texIn = glGetUniformLocation(gh->sp->getID(), "texture");
    glUniform1i(texIn, 0);
    SDL::SDL_Platform::CheckGL();
  }

  glDisable(GL_DEPTH_TEST);
  SDL::SDL_Platform::CheckGL();

  glDrawElements(GL_TRIANGLES,    // mode
                 gh->Indices,     // count
                 GL_UNSIGNED_INT, // type
                 (void*)0         // element array buffer offset
                 );
  SDL::SDL_Platform::CheckGL();

  // unbind VAO
  glBindVertexArray(0);
  SDL::SDL_Platform::CheckGL();

  // Unbind program
  glUseProgram(NULL);
  SDL::SDL_Platform::CheckGL();
}

// Called by Rocket when it wants to release application-compiled geometry.
void CLibRocketRenderInterface::ReleaseCompiledGeometry(
    Rocket::Core::CompiledGeometryHandle ROCKET_UNUSED_PARAMETER(geometry)) {
  ROCKET_UNUSED(geometry);
}

// Called by Rocket when it wants to enable or disable scissoring to clip
// content.
void CLibRocketRenderInterface::EnableScissorRegion(bool enable) {
  if (enable)
    glEnable(GL_SCISSOR_TEST);
  else
    glDisable(GL_SCISSOR_TEST);
}

// Called by Rocket when it wants to change the scissor region.
void CLibRocketRenderInterface::SetScissorRegion(int x, int y, int width,
                                                 int height) {
  
  glScissor(x, 720 - (y + height), width, height);
  
}

// Set to byte packing, or the compiler will expand our struct, which means it
// won't read correctly from file
#pragma pack(1)
struct TGAHeader {
  char idLength;
  char colourMapType;
  char dataType;
  short int colourMapOrigin;
  short int colourMapLength;
  char colourMapDepth;
  short int xOrigin;
  short int yOrigin;
  short int width;
  short int height;
  char bitsPerPixel;
  char imageDescriptor;
};
// Restore packing
#pragma pack()

// Called by Rocket when a texture is required by the library.
bool CLibRocketRenderInterface::LoadTexture(
    Rocket::Core::TextureHandle& texture_handle,
    Rocket::Core::Vector2i& texture_dimensions,
    const Rocket::Core::String& source) {

  Rocket::Core::FileInterface* file_interface =
      Rocket::Core::GetFileInterface();
  Rocket::Core::FileHandle file_handle = file_interface->Open(source);
  if (!file_handle) {
    return false;
  }

  file_interface->Seek(file_handle, 0, SEEK_END);
  size_t buffer_size = file_interface->Tell(file_handle);
  file_interface->Seek(file_handle, 0, SEEK_SET);

  ASSERT_MSG(buffer_size > sizeof(TGAHeader),
             "Texture file size is smaller than TGAHeader, file must be "
             "corrupt or otherwise invalid");
  if (buffer_size <= sizeof(TGAHeader)) {
    file_interface->Close(file_handle);
    return false;
  }

  char* buffer = new char[buffer_size];
  file_interface->Read(buffer, buffer_size, file_handle);
  file_interface->Close(file_handle);

  TGAHeader header;
  memcpy(&header, buffer, sizeof(TGAHeader));

  int color_mode = header.bitsPerPixel / 8;
  int image_size =
      header.width * header.height * 4; // We always make 32bit textures

  if (header.dataType != 2) {
    Rocket::Core::Log::Message(
        Rocket::Core::Log::LT_ERROR,
        "Only 24/32bit uncompressed TGAs are supported.");
    return false;
  }

  // Ensure we have at least 3 colors
  if (color_mode < 3) {
    Rocket::Core::Log::Message(Rocket::Core::Log::LT_ERROR,
                               "Only 24 and 32bit textures are supported");
    return false;
  }

  const char* image_src = buffer + sizeof(TGAHeader);
  unsigned char* image_dest = new unsigned char[image_size];

  // Targa is BGR, swap to RGB and flip Y axis
  for (long y = 0; y < header.height; y++) {
    long read_index = y * header.width * color_mode;
    long write_index =
        ((header.imageDescriptor & 32) != 0)
            ? read_index
            : (header.height - y - 1) * header.width * color_mode;
    for (long x = 0; x < header.width; x++) {
      image_dest[write_index] = image_src[read_index + 2];
      image_dest[write_index + 1] = image_src[read_index + 1];
      image_dest[write_index + 2] = image_src[read_index];
      if (color_mode == 4)
        image_dest[write_index + 3] = image_src[read_index + 3];
      else
        image_dest[write_index + 3] = 255;

      write_index += 4;
      read_index += color_mode;
    }
  }

  texture_dimensions.x = header.width;
  texture_dimensions.y = header.height;

  bool success =
      GenerateTexture(texture_handle, image_dest, texture_dimensions);

  delete[] image_dest;
  delete[] buffer;

  return success;
}

// Called by Rocket when a texture is required to be built from an
// internally-generated sequence of pixels.
bool CLibRocketRenderInterface::GenerateTexture(
    Rocket::Core::TextureHandle& texture_handle,
    const Rocket::Core::byte* source,
    const Rocket::Core::Vector2i& source_dimensions) {

  GLuint texture_id = 0;
  // We could push this thorugh the sld pipe, maybe later
  // Have OpenGL generate a texture object handle for us
  glGenTextures(1, &texture_id);
  SDL::SDL_Platform::CheckGL();
  ASSERT(texture_id != -1);

  // Bind the texture object
  glBindTexture(GL_TEXTURE_2D, texture_id);
  SDL::SDL_Platform::CheckGL();

  // Set the texture's stretching properties
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  SDL::SDL_Platform::CheckGL();

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, source_dimensions.x,
               source_dimensions.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, source);
  SDL::SDL_Platform::CheckGL();

  texture_handle = (Rocket::Core::TextureHandle)texture_id;

  return true;
}

// Called by Rocket when a loaded texture is no longer required.
void CLibRocketRenderInterface::ReleaseTexture(
    Rocket::Core::TextureHandle texture_handle) {
  /*
  glDeleteTextures(1, (GLuint*)&texture_handle);
  */
}
}