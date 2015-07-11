#ifndef __SHADERPROGRAM__
#define __SHADERPROGRAM__

#include <SFML/OpenGL.hpp>

#include <glm/glm.hpp>

#include <string>
#include <vector>


class ShaderProgram
{
private:

	static ShaderProgram* _activeProgram;
	GLuint _programID;
	std::vector<GLuint> _attachedShaderList;

	std::string ReadFile( std::string filename );
	bool AddShader( std::string file, GLenum shaderType );
	void Link();

public:

	ShaderProgram();
	~ShaderProgram();

	static ShaderProgram* glGetActiveProgram()
	{
		return _activeProgram;
	}

	ShaderProgram* AddVertexShader( std::string file );
	ShaderProgram* AddFragmentShader( std::string file );
	ShaderProgram* SetParameter( std::string varName, GLfloat* varValue );

	void Begin();
	void End();
	GLuint getID();




};

#endif
