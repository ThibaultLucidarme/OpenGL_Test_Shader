
#include "Mesh.hpp"

#include "ShaderProgram.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp> // print glm
#include <iostream>
#include <algorithm>


using namespace std;



Mesh::Mesh( void )
{

	//vao
	glGenVertexArrays( 1, &_vao );
	glBindVertexArray( _vao );
	_isLoadedToGPU = false;

}

Mesh::Mesh( string filename ) : Mesh()
{
	LoadOBJFromFile( filename );
}

Mesh::~Mesh( void )
{
	_isLoadedToGPU = false;
	//release vbos and vao
    glDisableVertexAttribArray(0); //attribIndex from Init
    glDisableVertexAttribArray(1);

    glDeleteBuffers(1, &_vertices_vbo);
    glDeleteBuffers(1, &_normals_vbo);
    glDeleteVertexArrays(1, &_vao);

}

void Mesh::LoadToGPU( bool useTexture )
{

	//points
	LoadArray( &_vertices_vbo, &_vertices, "vertex_position" );
	//normals
	LoadArray( &_normals_vbo, &_normals, "vertex_normal" );
	//color
	// LoadArray( &_colours_vbo, &_colours, "vertex_colour" );
	
	//uv
	if( useTexture ) {}


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

	if( !_isLoadedToGPU )
	{	
		LoadToGPU();
		_isLoadedToGPU = true;
	}

	glBindVertexArray( _vao );
	// draw all points from the currently bound VAO with current in-use shader
	glDrawArrays( mode, 0, _vertices.size() );


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


Mesh* Mesh::Move( glm::mat4 ModelMatrix )
{

	ShaderProgram::glGetActiveProgram()->SetParameter( "ModelMatrix", ModelMatrix );

	return this;

}


void Mesh::LoadOBJFromFile( string filename )
{

	FILE* file = fopen ( filename.c_str() , "r");

	char line[128];
	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector< glm::vec3 > temp_vertices, temp_normals;
	std::vector< glm::vec2 > temp_uvs;	
	
	_center_gravity = glm::vec3(0,0,0);
	top=-9999, right=-9999, bot=9999, left=9999, front=9999, back = -9999;


	while( !feof( file ) )	
	{
		// read the first word of the line
		int res = fscanf(file, "%s", line);
		if (res == EOF) break; // EOF = End Of File. Quit the loop.

		// vertices
		if ( strcmp( line, "v" ) == 0 )
		{
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
			temp_vertices.push_back(vertex);
		}
		
		// normals
		else if ( strcmp( line, "vn" ) == 0 )
		{
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
			temp_normals.push_back(normal);			
		}
		
		// uvs
		else if ( strcmp( line, "vt" ) == 0 )
		{
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y );
			temp_uvs.push_back(uv);			
		}
		
		// faces
		else if ( strcmp( line, "f" ) == 0 )
		{
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%u/%u/%u %u/%u/%u %u/%u/%u\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
			
			if (matches != 9) 
			{
				cerr<<"Incorrect number of face inputs"<<endl<<"Export with other options"<<endl<<"Skipping face"<<endl;
				break;
			}
			
			vertexIndices.push_back(vertexIndex[0]-1);
			vertexIndices.push_back(vertexIndex[1]-1);
			vertexIndices.push_back(vertexIndex[2]-1);
			uvIndices    .push_back(uvIndex[0]-1);
			uvIndices    .push_back(uvIndex[1]-1);
			uvIndices    .push_back(uvIndex[2]-1);
			normalIndices.push_back(normalIndex[0]-1);
			normalIndices.push_back(normalIndex[1]-1);
			normalIndices.push_back(normalIndex[2]-1);
		}
		
	}


	// indexing
	for_each( vertexIndices.begin(), vertexIndices.end(), [&](unsigned int vertexIndex)
		{
			_vertices.push_back( temp_vertices[ vertexIndex ] );
			_center_gravity += temp_vertices[ vertexIndex ];

			if (temp_vertices[ vertexIndex ].x>right) right = temp_vertices[ vertexIndex ].x;
			if (temp_vertices[ vertexIndex ].x<left) left = temp_vertices[ vertexIndex ].x;
			if (temp_vertices[ vertexIndex ].y>top) top = temp_vertices[ vertexIndex ].y;
			if (temp_vertices[ vertexIndex ].y<bot) bot = temp_vertices[ vertexIndex ].y;
			if (temp_vertices[ vertexIndex ].z>back) back = temp_vertices[ vertexIndex ].z;
			if (temp_vertices[ vertexIndex ].z<front) front = temp_vertices[ vertexIndex ].z;
		});
	for_each( normalIndices.begin(), normalIndices.end(), [&](unsigned int normalIndex)
		{
			_normals.push_back(  temp_normals[ normalIndex ] );
		});
	
	for_each( uvIndices.begin(), uvIndices.end(), [&](unsigned int uvIndex)
		{
			_uv.push_back(  temp_uvs[ uvIndex ] );
		});


	if(_vertices.size()) _center_gravity /= _vertices.size();
	cout<< glm::to_string(_center_gravity) <<endl;

	Normalize();


	fclose( file );
}



	Mesh* Mesh::Normalize( void )
	{
		cerr<<"Normalize"<<endl;

		//TODO
		glm::vec3 center = 0.5*glm::vec3(right+left, top+bot, front+back);
		glm::vec3 scale = glm::vec3(right-left, top-bot, back-front);

		for_each( _vertices.begin(), _vertices.end(), [&](glm::vec3 vertex) { 
			vertex -= center; 
			vertex *= scale;
		});

		return this;
	}

	Mesh* Mesh::Scale( float x, float y, float z )
	{
		// TODO
		return this;
	}


