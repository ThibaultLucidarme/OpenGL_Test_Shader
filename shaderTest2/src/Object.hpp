#ifndef __OBJECT__
#define __OBJECT__


#include "Mesh.hpp"
#include <vector>
#include <string>
#include <fstream>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


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

	Object( void );
	~Object( void );
	Object( std::string filename, GLfloat scale = 1.0f );
	Object* AttachMesh( Mesh* mesh );
	Object* Move( GLfloat qtty );
	Object* Draw( GLenum mode = GL_TRIANGLES, GLfloat rasterSize = 1.0 );
	glm::vec3 GetPosition( void );

	/*

	Object* AttachCamera( Camera* cam, vec3* offset = NULL );
	Object( string filename, vec3 scale =vec3(1.0f) );

//*/

};


#endif