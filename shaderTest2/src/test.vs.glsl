#version 410

in vec3 vertex_position;
in vec3 vertex_normal;

uniform mat4 ProjectionViewMatrix;
uniform mat4 ModelMatrix;

out vec3 normal;

void main() 
{

	normal = vertex_normal;
	gl_Position = ProjectionViewMatrix * ModelMatrix * vec4(vertex_position, 1.0);

}