
#include <SFML/OpenGL.hpp>


class Mesh
{
private:


	GLuint points_vbo;	
	GLuint colours_vbo;
	GLuint vao;


public:

	Mesh();
	void Draw();
	
};