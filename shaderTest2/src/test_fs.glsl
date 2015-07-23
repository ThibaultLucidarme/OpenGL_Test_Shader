#version 410

in vec3 normal;

out vec4 frag_colour;

void main() 
{
	frag_colour = vec4( normal, 1.0 );
}