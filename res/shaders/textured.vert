#version 430

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 LVertexPos;
layout(location = 2) in vec2 uvs;
// Values that stay constant for the whole mesh.
uniform mat4 viewprojection;
uniform mat4 modelprojection;

out vec2 UV;

void main()
{
  gl_Position =  viewprojection * modelprojection * vec4(LVertexPos,1);
  UV = uvs;
}
