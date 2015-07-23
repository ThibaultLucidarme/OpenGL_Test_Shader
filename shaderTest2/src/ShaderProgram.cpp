
#include "ShaderProgram.hpp"
#include <fstream>
#include <sstream>
#include <iostream>


using namespace std;


ShaderProgram* ShaderProgram::_activeProgram = NULL;


ShaderProgram::ShaderProgram( void )
{
	_programID = glCreateProgram();

}

ShaderProgram::~ShaderProgram( void )
{
	for( GLuint &shaderID : _attachedShaderList )
	{
		glDetachShader( _programID, shaderID );
		glDeleteShader( shaderID );
	}

    glDeleteProgram( _programID );

}

string ShaderProgram::ReadFile( string filename )
{
	stringstream stream;
	string line;

	ifstream file( filename );
	if (file.is_open())
	{
		while ( getline( file,line ) ) 
			stream << line << '\n';
		
		file.close();
	}

	return stream.str();
	
}

bool ShaderProgram::AddShader( string file, GLenum shaderType )
{
	GLuint shaderID = glCreateShader( shaderType );
	string shaderContent = ReadFile( file );

	const GLchar* content = const_cast<const GLchar*>( shaderContent.c_str() );
	glShaderSource( shaderID, 1, &content, NULL );
	glCompileShader( shaderID );
	glAttachShader( _programID, shaderID );
	
	// check for compile errors
	int compileSuccess = -1;
	glGetShaderiv( shaderID, GL_COMPILE_STATUS, &compileSuccess );
	if( compileSuccess != GL_TRUE ) cerr<<"ERROR: Could not compile "<<file<<endl;
	else _attachedShaderList.push_back( shaderID );


	// XXX TEMP XXX
	// only needed once after last AddShader call instead of after every AddShader
	Link();

	return static_cast<bool>(compileSuccess);

}

ShaderProgram* ShaderProgram::AddVertexShader( string file )
{
	AddShader( file, GL_VERTEX_SHADER );
	return this;

}

ShaderProgram* ShaderProgram::AddFragmentShader( string file )
{
	AddShader( file, GL_FRAGMENT_SHADER );
	return this;

}

void ShaderProgram::Begin( void )
{
	_activeProgram = this;
	glUseProgram( _programID );

}

void ShaderProgram::End( void )
{
	_activeProgram = NULL;
	glUseProgram( 0 );

}

GLuint ShaderProgram::getID( void )
{
	return _programID;

}

void ShaderProgram::Link( void )
{
	glLinkProgram( _programID ); 

	// check for linking errors
	int linkSuccess = -1;
	glGetProgramiv( _programID, GL_LINK_STATUS, &linkSuccess );
	if( linkSuccess != GL_TRUE ) cerr<<"ERROR: Could not link program"<<endl;

}

