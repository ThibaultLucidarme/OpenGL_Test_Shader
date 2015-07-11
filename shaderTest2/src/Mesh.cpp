
#include "Mesh.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>


using namespace std;



Mesh::Mesh( void )
{

	//vao
	glGenVertexArrays( 1, &_vao );
	glBindVertexArray( _vao );

}

Mesh::Mesh( string filename ) : Mesh()
{
	LoadFromFile( filename );
}

Mesh::~Mesh( void )
{

	//release vbos and vao
    glDisableVertexAttribArray(0); //attribIndex from Init
    glDisableVertexAttribArray(1);

    glDeleteBuffers(1, &_vertices_vbo);
    glDeleteBuffers(1, &_colours_vbo);
    glDeleteVertexArrays(1, &_vao);

}

void Mesh::LoadToGPU( bool useTexture )
{

	//points
	LoadArray( &_vertices_vbo, &_vertices, "vertex_position" );
	//color
	LoadArray( &_colours_vbo, &_colours, "vertex_colour" );
	//normals
	
	//uv
	if( useTexture ) {}

	// if( combination[0] ) Init( &_vertices_vbo, _vertices, 9 );
	// if( combination[1] ) Init( &_normals_vbo, _normals, 9 );
	// if( combination[2] ) Init( &_indices_vbo, _indices, 9 );
	// if( combination[3] ) Init( &_uv_vbo, _uv, 9 );

}

void Mesh::LoadArray( GLuint* vbo, std::vector<glm::vec3>* array, string varName )
{
	
	GLuint varLocation = glGetAttribLocation( ShaderProgram::glGetActiveProgram()->getID(), varName.c_str() );
	GLuint length = array->size()*sizeof(array[0][0]); 


	glGenBuffers( 1, vbo );
	glBindBuffer( GL_ARRAY_BUFFER, *vbo );
	glBufferData( GL_ARRAY_BUFFER, length, &(array[0][0]), GL_STATIC_DRAW);
	glVertexAttribPointer( varLocation, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray( varLocation );

}

void Mesh::Draw( GLenum mode, GLfloat rasterSize)
{

	LoadToGPU();
	
	glBindVertexArray( _vao );
	// draw all points from the currently bound VAO with current in-use shader
	glDrawArrays( GL_LINE_LOOP, 0, _vertices.size() );
	

	switch( mode )
	{
		case GL_POINTS:
			glPointSize( rasterSize );
			break;
		case GL_LINE_STRIP:
		case GL_LINE_LOOP:
		case GL_LINES:
		case GL_LINE_STRIP_ADJACENCY:
		case GL_LINES_ADJACENCY:
			glLineWidth( rasterSize );
			break;
		default:
			;

	}


}

void Mesh::LoadFromFile( string file )
{
	_vertices.push_back( glm::vec3(  0.0f,  0.5f, 0.0f ) );
	_vertices.push_back( glm::vec3(  0.5f, -0.5f, 0.0f ) );
	_vertices.push_back( glm::vec3( -0.5f, -0.5f, 0.0f ) );

	_colours.push_back( glm::vec3( 1.0f, 0.0f, 0.0f ) );
	_colours.push_back( glm::vec3( 0.0f, 1.0f, 0.0f ) );
	_colours.push_back( glm::vec3( 0.0f, 0.0f, 1.0f ) );
//*/
/*
	_vertices.push_back( glm::vec3(  0.-5f,  0.5f, 0.0f ) );
	_vertices.push_back( glm::vec3(  0.5f, 0.5f, 0.0f ) );
	_vertices.push_back( glm::vec3( 0.5f, -0.5f, 0.0f ) );
	_vertices.push_back( glm::vec3( -0.5f, -0.5f, 0.0f ) );

	_vertices.push_back( glm::vec3(  0.-5f,  0.5f, 0.0f ) );
	_vertices.push_back( glm::vec3(  0.5f, 0.5f, 0.0f ) );
	_vertices.push_back( glm::vec3( 0.5f, -0.5f, 0.0f ) );
	_vertices.push_back( glm::vec3( -0.5f, -0.5f, 0.0f ) );

	_colours.push_back( glm::vec3( 0.1f, 0.0f, 0.0f ) );
	_colours.push_back( glm::vec3( 0.0f, 1.0f, 0.0f ) );
	_colours.push_back( glm::vec3( 0.0f, 0.0f, 1.0f ) );

	//*/

}

Mesh* Mesh::Move( glm::mat4 MVmatrix )//, glm::mat4 Pmatrix )
{
	// MV = Model View
	// /!\ Multiply M and V on CPU (vertex independant) and MV with P on GPU (Projection is vertex dependant)

	// TODO
	// calculate ModelView from Object position and camera
	// change SetParameter to template type glm::mat4
	ShaderProgram::glGetActiveProgram()->SetParameter( "MVmatrix", glm::value_ptr(MVmatrix) );

	// calculate projection from camera
	// ShaderProgram::glGetActiveProgram()->SetParameter("Pmatrix", Pmatrix);

	return this;

}




