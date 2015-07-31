#ifndef __MESH__
#define __MESH__


#include <SFML/OpenGL.hpp>
#include <vector>
#include <string>
#include <fstream>
#include "ShaderProgram.hpp"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#define MESH_DYNAMIC true
#define MESH_STATIC false


class Mesh
{
private:

	GLuint _vertices_vbo;
	std::vector<glm::vec3> _vertices;

	GLuint _colours_vbo;
	std::vector<glm::vec3> _colours;

	GLuint _normals_vbo;
	std::vector<glm::vec3> _normals;
	
	GLuint _indices_vbo;
	std::vector<glm::vec3> _indices;
	
	GLuint _uv_vbo;
	std::vector<glm::vec2> _uv;

	std::vector<GLfloat> _flatArray;


	GLuint _vao;


	void LoadArray( GLuint* vbo, std::vector<glm::vec3>* array, std::string varName);
	void LoadToGPU( bool useTexture = false );


public:

	Mesh( void );
	Mesh( std::string filename );
	~Mesh();

	void LoadFromFile( void );
	void LoadOBJFromFile( std::string file );
	void Draw( GLenum mode = GL_TRIANGLES, GLfloat rasterSize = 1.0 );
	Mesh* Move( glm::mat4 ModelMatrix );
	
};




//
// TODO:
// Make this class abstract so that the real ship class inherits from it
//

class Object
{
private: 

	glm::mat4 _model;
	Mesh* _mesh;


public:

	Object()
	{
		//If there is a single scalar parameter to a matrix constructor, it is used to initialize all the components on the matrix's diagonal, with the remaining components initialized to 0.0f

	}

	Object( std::string filename ) : Object()
	{
		_model = glm::mat4( 1.0 );
		AttachMesh( new Mesh( filename ) );
	}
	
	Object* AttachMesh( Mesh* mesh )
	{
		_mesh = mesh;

		return this;
	}

	Object* Move( GLfloat qtty )
	{
		// Do not call Mesh::Move()
		// offset it to when Object::Draw()
		// This is so that the Object::Move is independent from the ShaderProgram::Begin context 
		// _model[1].x = qtty;

		_model = glm::rotate( _model, qtty, glm::vec3(0.1, 1.0, 0.2) );

		return this;
	}

	Object* Draw( GLenum mode = GL_TRIANGLES, GLfloat rasterSize = 1.0 )
	{
		// calculate ViewProjection matrix
		_mesh->Move( _model );
		_mesh->Draw( mode, rasterSize );

		return this;
	}

	glm::vec3 GetPosition( void )
	{
		return  glm::vec3( _model[3].x, _model[3].y, _model[3].z  );
	}

	/*

	Object* AttachCamera( Camera* cam, vec3* offset = NULL )
	{
		_attachedCamera = cam;
	if(offset!=NULL) _cameraOffset = *offset; // else use _cameraOffset defined in Object constructor
	
	_attachedCamera -> MoveTo( _position+_cameraOffset );
}


Object::Object( string filename, vec3 scale =vec3(1.0f) )
{
	// read filename
	// and loadMesh
	
	_model = glm::scale( _model, scale );
}

//*/

};


#endif