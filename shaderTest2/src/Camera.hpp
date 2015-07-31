#ifndef __CAMERA__
#define __CAMERA__


#include <SFML/OpenGL.hpp>
#include <vector>
#include <string>
#include <fstream>

#define GLM_FORCE_RADIANS
#define GLM_SWIZZLE // mat4.xyz()
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum CameraMode{
	CAM_ORTHO,
	CAM_PERSP,
	CAM_FRUSTUM,
	
	CAM_CART,
	CAM_SPHERE
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
	GLfloat _aspectRatio;
	enum CameraMode _camMode;

	void CalculateProjection( void )
	{

		switch( _camMode )
		{
			case CAM_ORTHO:
			_projection = glm::ortho(
					-250.f, // left
					250.f, // right,
					-250.f, // bottom,
					250.f, // top,
					0.1f, //near plane
					100.f //far plane
					);
			break;	
			
			case CAM_FRUSTUM:
			_projection = glm::frustum(
					-250.f, // left
					250.f, // right,
					-250.f, // bottom,
					250.f, // top,
					0.1f, //near plane
					100.f //far plane
					);
			break;
			
			case CAM_PERSP:
			default:
			_projection = glm::perspective( 
				_fov,
				_aspectRatio,
					0.1f, //near plane
					100.f //far plane
					);
			
		}

	}
	
	

public:

	Camera( enum CameraMode mode, glm::vec3 pos = glm::vec3(1.0, 1.0, 1.0), glm::vec3 lookat = glm::vec3(0.0, 0.0, 0.0)  )
	{
		
		_fov = 90;
		_aspectRatio = 4.f/3.f;
		_camMode = mode;
		_position = pos;
		_lookAt = lookat;
		
		CalculateProjection();
		
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
		glm::vec3 displacement = _position-newPosition;
		_position = newPosition;
		if(!stayFocused) _lookAt += displacement;
		return this;
	}
	
	Camera* SetCoordinates( enum CameraMode mode  )
	{
		switch( mode )
		{
			case CAM_SPHERE:
			;
			break;
			case CAM_CART:
			default:
			;
		}
		
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


	Camera* Rotate( glm::vec3 axis, GLfloat angle, glm::vec3 pivot = glm::vec3(1.0f) )
	{
		glm::mat4 rotation = glm::rotate( glm::mat4(1.0f), angle, glm::normalize(axis) );
		_position = ( rotation * glm::vec4( _position-pivot, 1.0) 
			+ glm::vec4(pivot,1.0)
			).xyz();

		return this;
	}


	Camera* Zoom( GLfloat qtty=1.0f )
	{
		_fov += qtty;
		CalculateProjection();
		return this;
	}


// Camera* Camera::AttachToObject(Object* o, vec3 offset=vec3(0.0f) )
// {
// 	o->AttachCamera( this, offset );
// }

	
};


Camera* Camera::_currentCam = NULL;

#endif
