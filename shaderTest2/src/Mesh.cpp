
#include "Mesh.hpp"
#include <iostream>

using namespace std;

Mesh::Mesh()
{

	//vao
	glGenVertexArrays( 1, &_vao );
	glBindVertexArray( _vao );


}

Mesh::~Mesh()
{

	//release vbos and vao

}

void Mesh::LoadToGPU( bool useTexture )
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
	Init(&_vertices_vbo, points, 9);
	//color
	Init(&colours_vbo, colours, 9);
	//normals
	
	//uv
	if( useTexture ) {}

	// if( combination[0] ) Init( &_vertices_vbo, _vertices, 9 );
	// if( combination[1] ) Init( &_normals_vbo, _normals, 9 );
	// if( combination[2] ) Init( &_indices_vbo, _indices, 9 );
	// if( combination[3] ) Init( &_uv_vbo, _uv, 9 );
}

//rename
void Mesh::Init( GLuint* vbo, const GLfloat* array, int size)
{
	static int index = 0;
	//points
	glGenBuffers( 1, vbo );
	glBindBuffer( GL_ARRAY_BUFFER, *vbo );
	glBufferData( GL_ARRAY_BUFFER, size * sizeof (GLfloat), array, GL_STATIC_DRAW);
	glVertexAttribPointer( index, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray( index );

	index++;
}

void Mesh::Draw()
{
	glBindVertexArray( _vao );
	// draw points 0-3 from the currently bound VAO with current in-use shader
	glDrawArrays( GL_TRIANGLES, 0, 3 );
}


void Mesh::LoadFromFile(string file)
{


}
