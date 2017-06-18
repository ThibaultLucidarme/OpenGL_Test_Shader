#ifndef __CAMERA__
#define __CAMERA__


#include <SFML/OpenGL.hpp>
#include <vector>
#include <string>
#include <fstream>
#include <exception>
#include <iostream>

#define GLM_FORCE_RADIANS
#define GLM_SWIZZLE // mat4.xyz()
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <functional>

#include "InputDevice.hpp"

enum CameraMode{
	CAM_ORTHO,
	CAM_PERSP,
	CAM_FRUSTUM,
	
	CAM_CART,
	CAM_SPHERE
};

class Camera: public InputDevice
{
private:

	glm::vec3 _position;
	glm::vec3 _lookAt;
	glm::mat4 _view;
	glm::mat4 _projection;

	sf::Vector2i _mouseDownPosition;
	
	static Camera* _currentCam;
	// default mouse events
	std::function< Camera* ( const sf::Event& )> _nullAction_default;
	std::function< Camera* ( const sf::Window& )> _nullState_default;
	

	GLfloat _fov;
	GLfloat _aspectRatio;
	enum CameraMode _camMode;

	void CalculateProjection( void );



public:

	Camera( enum CameraMode mode, glm::vec3 pos = glm::vec3(1.0, 1.0, 1.0), glm::vec3 lookat = glm::vec3(0.0, 0.0, 0.0)  );
	~Camera( void );
	
	Camera* Use( void );
	Camera* SetMode( enum CameraMode mode);
	Camera* Move( glm::vec3 displacement, bool stayFocused = false );
	Camera* MoveTo( glm::vec3 newPosition, bool stayFocused = true );
	Camera* SetCoordinates( enum CameraMode mode  );
	Camera* FocusOn( glm::vec3 target );
	glm::mat4 GetProjectionView( void );
	Camera* Rotate( glm::vec3 axis, GLfloat angle, glm::vec3 pivot = glm::vec3(1.0f) );
	Camera* Zoom( GLfloat qtty=1.0f );

	static Camera* GetActiveCamera();

	Camera* HandleActiveEvent( sf::Event& event);
	//active mouse events
	std::function< Camera* ( const sf::Event& )> onLeftClick;
	std::function< Camera* ( const sf::Event& )> onRightClick;
	std::function< Camera* ( const sf::Event& )> onLeftRelease;
	std::function< Camera* ( const sf::Event& )> onRightRelease;
	std::function< Camera* ( const sf::Event& )> onMiddleClick;
	std::function< Camera* ( const sf::Event& )> onMiddleRelease;
	std::function< Camera* ( const sf::Event& )> onScroll;


	Camera* HandlePassiveState( sf::Window& window);
	//passive mouse events
	std::function< Camera* ( const sf::Window& )> whileLeftPressed;
	std::function< Camera* ( const sf::Window& )> whileRightPressed;
	std::function< Camera* ( const sf::Window& )> whileLeftReleased;
	std::function< Camera* ( const sf::Window& )> whileRightReleased;
	std::function< Camera* ( const sf::Window& )> whileMiddlePressed;
	std::function< Camera* ( const sf::Window& )> whileMiddleReleased;


// Camera* Camera::AttachToObject(Object* o, vec3 offset=vec3(0.0f) )
// {
// 	o->AttachCamera( this, offset );
// }

	
};



#endif
