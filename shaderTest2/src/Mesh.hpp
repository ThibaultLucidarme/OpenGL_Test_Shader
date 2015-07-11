#ifndef __MESH__
#define __MESH__


#include <SFML/OpenGL.hpp>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <iostream>
#include "ShaderProgram.hpp"


#define MESH_DYNAMIC true
#define MESH_STATIC false


class Mesh
{
private:

	GLuint _vertices_vbo;
	std::vector<glm::vec3> _vertices;

	GLuint _normals_vbo;
	std::vector<glm::vec3> _normals;
	
	GLuint _indices_vbo;
	std::vector<glm::vec3> _indices;
	
	GLuint _uv_vbo;
	std::vector<glm::vec2> _uv;


	GLuint colours_vbo;
	GLuint _vao;


	void Init( GLuint* vbo, const GLfloat* array, int size, int index);
	void SetInput( GLuint* vbo, const GLfloat* array, int size, std::string varName );
	void LoadToGPU( bool useTexture = false );
public:

	Mesh();
	Mesh( std::string filename );
	~Mesh();

	void LoadFromFile( std::string file);
	void Draw( );
	Mesh* Move( GLfloat* MVmatrix );
	
};




//
// TODO:
// Make this class abstract so that the real ship class inherits from it
//

class Ship
{
private: 

	GLfloat _model[16];
	Mesh* _mesh;


public:

	Ship()
	{
		_model[0] = 1.0f;
		_model[1] = 0.0f;
		_model[2] = 0.0f;
		_model[3] = 0.0f;
		_model[4] = 0.0f;
		_model[5] = 1.0f;
		_model[6] = 0.0f;
		_model[7] = 0.0f;
		_model[8] = 0.0f;
		_model[9] = 0.0f;
		_model[10] = 1.0f;
		_model[11] = 0.0f;
		_model[12] = 0.0f;
		_model[13] = 0.0f;
		_model[14] = 0.0f;
		_model[15] = 1.0f;
	}
	
	void AttachMesh( Mesh* mesh )
	{
		_mesh = mesh;
	}

	void Move( GLfloat qtty )
	{
		// Do not call Mesh::Move()
		// offset it to when Ship::Draw()
		// This is so that the Ship::Move is independent from the ShaderProgram::Begin context 
		_model[1] = qtty;
	}

	void Draw()
	{
		_mesh->Move( _model );
		_mesh->Draw();
	}

};


#endif