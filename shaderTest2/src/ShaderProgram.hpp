#ifndef __SHADERPROGRAM__
#define __SHADERPROGRAM__

#include <SFML/OpenGL.hpp>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

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

	template<typename T>
	ShaderProgram* SetParameter( std::string varName, T varValue )
	{
		int varLocation = glGetUniformLocation( _programID, varName.c_str() );
		glUseProgram( _programID );
		glUniformMatrix4fv( varLocation, 1, GL_FALSE, glm::value_ptr(varValue) );

		return this;

	}

	void Begin();
	void End();
	GLuint getID();




};

#endif
