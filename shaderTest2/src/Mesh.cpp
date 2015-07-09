
#include "Mesh.hpp"

using namespace std;

Mesh::Mesh()
{

	const GLfloat points[] = {
		 0.0f,	0.5f,	0.0f,
		 0.5f, -0.5f,	0.0f,
		-0.5f, -0.5f,	0.0f
	};
	
	const GLfloat colours[] = {
		1.0f, 0.0f,  0.0f,
		0.0f, 1.0f,  0.0f,
		0.0f, 0.0f,  1.0f
	};


	//points
	glGenBuffers (1, &points_vbo);
	glBindBuffer (GL_ARRAY_BUFFER, points_vbo);
	glBufferData (GL_ARRAY_BUFFER, 9 * sizeof (GLfloat), points, GL_STATIC_DRAW);

	//color
	glGenBuffers (1, &colours_vbo);
	glBindBuffer (GL_ARRAY_BUFFER, colours_vbo);
	glBufferData (GL_ARRAY_BUFFER, 9 * sizeof (GLfloat), colours, GL_STATIC_DRAW);
	
	//vao
	glGenVertexArrays (1, &vao);
	glBindVertexArray (vao);
	glBindBuffer (GL_ARRAY_BUFFER, points_vbo);
	glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer (GL_ARRAY_BUFFER, colours_vbo);
	glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray (0);
	glEnableVertexAttribArray (1);

}

void Mesh::Draw()
{
	glBindVertexArray( vao );
	// draw points 0-3 from the currently bound VAO with current in-use shader
	glDrawArrays( GL_TRIANGLES, 0, 3 );
}