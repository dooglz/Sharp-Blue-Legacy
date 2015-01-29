#version 430 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec2 LVertexPos2D;
layout(location = 1) in vec4 vertexColor;

layout(location = 0) uniform vec2 transform;
layout(location = 1) uniform mat4 projection;

out vec4 fragmentColor;

void main(){	
	gl_Position =  projection * vec4(transform+LVertexPos2D,0,1);
	fragmentColor = vertexColor;
}
