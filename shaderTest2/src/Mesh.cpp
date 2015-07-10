
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
    glDisableVertexAttribArray(0); //attribIndex from Init
    glDisableVertexAttribArray(1);
   
    glDeleteBuffers(2, vbo);
    glDeleteVertexArrays(1, &vao);

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
	Init(&_vertices_vbo, points, 9, 0);
	//color
	Init(&colours_vbo, colours, 9, 1);
	//normals
	
	//uv
	if( useTexture ) {}

	// if( combination[0] ) Init( &_vertices_vbo, _vertices, 9 );
	// if( combination[1] ) Init( &_normals_vbo, _normals, 9 );
	// if( combination[2] ) Init( &_indices_vbo, _indices, 9 );
	// if( combination[3] ) Init( &_uv_vbo, _uv, 9 );
}

//rename
void Mesh::SetInput( GLuint* vbo, const GLfloat* array, int size, string varName)
{
	/*
	varLocation is the value of the "layout (location=`varLocation`)" in the vertex shader for the variable varName
	 
	 
	If the layout information is not in the shader then openGL will create a default layout 
	(usually in order as they appear) or you can define your own binding with 
	glBindAttribLocation(progam, id, name) before linking.

	You can explicitly get those numbers from id = glGetAttribLocation(program, name); 
	after linking; the name is the string that appears in the vertex shader for the attribute. 
	For example if you passed "vertexPosition" for name then you would get 0.
	
	

	*/
	int varLocation = glGetAttribLocation( _programID, varName.c_str() );
	
	
	glGenBuffers( 1, vbo );
	glBindBuffer( GL_ARRAY_BUFFER, *vbo );
	glBufferData( GL_ARRAY_BUFFER, size * sizeof (GLfloat), array, GL_STATIC_DRAW);
	glVertexAttribPointer( varLocation, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray( varLocation );
}


void Mesh::Init( GLuint* vbo, const GLfloat* array, int size, int index)
{
	/*
	index is the value of the "layout (location=`index`)" in the vertex shader for the attribute
	 
	 
	If the layout information is not in the shader then openGL will create a default layout 
	(usually in order as they appear) or you can define your own binding with 
	glBindAttribLocation(progam, id, name) before linking.

	You can explicitly get those numbers from id = glGetAttribLocation(program, name); 
	after linking; the name is the string that appears in the vertex shader for the attribute. 
	For example if you passed "vertexPosition" for name then you would get 0.
	
	

	*/
	
	glGenBuffers( 1, vbo );
	glBindBuffer( GL_ARRAY_BUFFER, *vbo );
	glBufferData( GL_ARRAY_BUFFER, size * sizeof (GLfloat), array, GL_STATIC_DRAW);
	glVertexAttribPointer( index, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray( index );
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
