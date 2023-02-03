#version 430 core

layout(location = 0) in vec3 vertexPosition;

uniform mat4 transformation;
uniform mat4 VIEW;


out vec3 texCoord;

void main()
{
	texCoord = vertexPosition;
	gl_Position = transformation * VIEW *vec4(vertexPosition, 1.0);
	
    
}