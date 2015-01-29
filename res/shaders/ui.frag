#version 330 core

// Interpolated values from the vertex shaders
in vec4 fragmentColor;

// Ouput data
out vec4 color;

void main(){
	color =  vec4(0.2f, 0.2f, 0.2f, 1.0f);
}
