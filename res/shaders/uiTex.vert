#version 430

layout(location = 0) in vec2 LVertexPos2D;
layout(location = 2) in vec2 uvs;

layout(location = 0) uniform vec2 transform;
layout(location = 1) uniform mat4 projection;

out vec2 UV;

void main()
{
  gl_Position =  projection * vec4(transform+LVertexPos2D,0,1);
  UV = uvs;
}
