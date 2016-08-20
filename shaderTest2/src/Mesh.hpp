#ifndef __MESH__
#define __MESH__


#include <SFML/OpenGL.hpp>
#include <vector>
#include <string>
#include <fstream>

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
	bool _isLoadedToGPU;


	void LoadArray( GLuint* vbo, std::vector<glm::vec3>* array, std::string varName);
	void LoadToGPU( bool useTexture = false );


public:

	Mesh( void );
	Mesh( std::string filename );
	~Mesh( void );

	void LoadOBJFromFile( std::string file );
	void Draw( GLenum mode = GL_TRIANGLES, GLfloat rasterSize = 1.0 );
	Mesh* Move( glm::mat4 ModelMatrix );
	Mesh* Normalize( void );
	Mesh* Scale( float, float, float );
	
};





#endif