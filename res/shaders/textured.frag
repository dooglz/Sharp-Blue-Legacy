#version 430

in vec2 UV;
layout(location = 50) uniform sampler2D texture;

out vec4 color;

void main()
{
	color = texture2D(texture, UV);
}