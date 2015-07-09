#include <SFML/OpenGL.hpp>
#include <string>

class ShaderProgram
{
private:

	GLuint _programID;

	std::string ReadFile( std::string filename );
	bool AddShader( std::string file, GLenum shaderType );
	void Link();

public:

	ShaderProgram();

	ShaderProgram* AddVertexShader( std::string file );
	ShaderProgram* AddFragmentShader( std::string file );
	ShaderProgram* SetParameter( std::string varName, GLfloat* varValue );

	void Begin();
	void End();
	GLuint getID();




};
