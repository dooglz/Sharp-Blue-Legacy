#version 430

in vec2 UV;
layout(location = 50) uniform sampler2D texture;

out vec4 color;

void main()
{
	//color = texture2D(texture, UV);
	color =  vec4(0.5f+(UV.x*0.5), 0.5f+(UV.y*0.5), 0.0f, 1.0f);
}