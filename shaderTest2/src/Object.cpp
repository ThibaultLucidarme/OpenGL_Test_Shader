#include "Object.hpp"
#include <iostream>

Object::Object( void )
{
		//If there is a single scalar parameter to a matrix constructor, it is used to initialize all the components on the matrix's diagonal, with the remaining components initialized to 0.0f
}

Object::~Object( void )
{
	delete _mesh;
}

Object::Object( std::string filename, GLfloat scale) : Object()
{
	_model = glm::scale(glm::mat4( 1.0f ), glm::vec3(scale) );
	AttachMesh( new Mesh( filename ) );
}


Object* Object::AttachMesh( Mesh* mesh )
{
	_mesh = mesh;

	return this;
}

Object* Object::Move( GLfloat qtty )
{
		// Do not call Mesh::Move()
		// offset it to when Object::Draw()
		// This is so that the Object::Move is independent from the ShaderProgram::Begin context 
		// _model[1].x = qtty;

	_model = glm::rotate( _model, qtty, glm::vec3(0.1, 1.0, 0.2) );

	return this;
}

Object* Object::Draw( GLenum mode, GLfloat rasterSize )
{
		// calculate ViewProjection matrix
	_mesh->Move( _model );
	_mesh->Draw( mode, rasterSize );

	return this;
}

glm::vec3 Object::GetPosition( void )
{
	return  glm::vec3( _model[3].x, _model[3].y, _model[3].z  );
}

	/*

	Object* Object::AttachCamera( Camera* cam, vec3* offset = NULL )
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