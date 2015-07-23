#ifndef __CAMERA__
#define __CAMERA__


#include <SFML/OpenGL.hpp>
#include <vector>
#include <string>
#include <fstream>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum CameraMode{
	CAM_ORTHO,
	CAM_PERSP,
	CAM_FRUSTUM
};

class Camera
{
private:

	glm::vec3 _position;
	glm::vec3 _lookAt;
	glm::mat4 _view;
	glm::mat4 _projection;
	
	static Camera* _currentCam;
	
	GLfloat _fov;
	enum CameraMode _camMode;
	
	

public:

	Camera( enum CameraMode mode, glm::vec3 pos = glm::vec3(1.0, 1.0, 1.0), glm::vec3 lookat = glm::vec3(0.0, 0.0, 0.0)  )
	{
		
		_camMode = mode;
		_position = pos;
		_lookAt = lookat;
		
		switch( _camMode )
		{
			case CAM_ORTHO:
			_projection = glm::ortho(
			 	-250, // left
				250, // right,
				-250, // bottom,
				250, // top,
				0.1f, //near plane
				100.f //far plane
			);
			break;
			
			case CAM_PERSP:
			_projection = glm::perspective( 
				_fov,
				_aspectRatio,
				0.1f, //near plane
				100.f //far plane
			);
			break;
					
			case CAM_FRUSTUM:
			_projection = glm::frustum(
				-250, // left
				250, // right,
				-250, // bottom,
				250, // top,
				0.1f, //near plane
				100.f //far plane
			);
			break;
			
		}
		
		Use();
	}
	
	Camera* Use( void )
	{
		_currentCam = this;
		return this;
	}
	
	Camera* SetMode( enum CameraMode mode)
	{
		_camMode = mode;
		return this;
	}
		
	Camera* Move( glm::vec3 displacement, bool stayFocused = false )
	{
		_position += displacement;
		if(!stayFocused) _lookAt += displacement;
		return this;
	}
	
	Camera* MoveTo( glm::vec3 newPosition, bool stayFocused = true )
	{
		_position = newPosition;
		if(!stayFocused) _lookAt += displacement;
		return this;
	}
	
	Camera* FocusOn( glm::vec3 target )
	{
		_lookAt = target;		
		return this;
	}
	
	glm::mat4 GetProjectionView( void )
	{
		_view = glm::lookAt( 
			_position,
			_lookAt,
			glm::vec3(0.0, 0.1, 0.0) //up
		);		

		return _projection * _view;
	}
	
	static Camera* GetActiveCamera()
	{
		return _currentCam;;
	}

	
};


Camera* Camera::_currentCam = NULL;
