#version 430 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec2 LVertexPos2D;
layout(location = 1) in vec4 vertexColor;

// Output data ; will be interpolated for each fragment.
out vec4 fragmentColor;

void main(){	
	gl_Position =  vec4(LVertexPos2D,0,1);
	fragmentColor = vertexColor;
}
