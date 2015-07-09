
#include <SFML/OpenGL.hpp>
#include <glm/glm.hpp>
#include <vector>
#include <string>


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


public:

	Mesh();
	~Mesh();
	void Draw();
	void Init( GLuint* vbo, const GLfloat* array, int size );
	void LoadFromFile( std::string file);
	void LoadToGPU( bool useTexture = false );
	
};