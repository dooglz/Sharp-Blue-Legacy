#version 430

layout(location = 0) in vec3 LVertexPos;
layout(location = 2) in vec2 uvs;

out vec2 UV;

void main()
{
  gl_Position =  vec4(LVertexPos2D,0,1);
  UV = uvs;
}
