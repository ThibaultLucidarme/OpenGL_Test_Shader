#ifndef __MESH__
#define __MESH__


#include <SFML/OpenGL.hpp>
#include <vector>
#include <string>
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

	Mesh();
	Mesh( std::string filename );
	~Mesh();

	void LoadFromFile( std::string file);
	void Draw( GLenum mode = GL_TRIANGLES, GLfloat rasterSize = 1.0 );
	Mesh* Move( glm::mat4 MVmatrix );
	
};




//
// TODO:
// Make this class abstract so that the real ship class inherits from it
//

class Ship
{
private: 

	glm::mat4 _model;
	Mesh* _mesh;


public:

	Ship()
	{
		//If there is a single scalar parameter to a matrix constructor, it is used to initialize all the components on the matrix's diagonal, with the remaining components initialized to 0.0f
		_model = glm::mat4( 1.0 );

	}

	Ship( std::string filename ) : Ship()
	{
		AttachMesh( new Mesh( filename ) );
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
		// _model[1].x = qtty;

		_model = glm::rotate( _model, qtty, glm::vec3(0.0, 0.0, 1.0) );
	}

	void Draw( GLenum mode = GL_TRIANGLES, GLfloat rasterSize = 1.0 )
	{
		_mesh->Move( _model );
		_mesh->Draw( mode, rasterSize );
	}

};


#endif