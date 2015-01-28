#version 430

layout(location = 2) in vec2 uv;
layout(location = 50) uniform sampler2D texture;

out vec4 color;

void main()
{
	color = texture2D(texture, uv);
}