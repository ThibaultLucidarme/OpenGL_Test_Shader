#include "Camera.hpp"


Camera* Camera::_currentCam = NULL;

	// Constructors ************************************************************************************

Camera::Camera( enum CameraMode mode, glm::vec3 pos, glm::vec3 lookat )
{

	_fov = 90;
	_aspectRatio = 4.f/3.f;
	_camMode = mode;
	_position = pos;
	_lookAt = lookat;



	_nullAction_default = [&, this](const sf::Event& event){return this;};
	_nullState_default = [&, this](const sf::Window& window){return this;};

	//active mouse events
	onLeftClick = [&, this](const sf::Event& event)
	{ 
		_mouseDownPosition.x = event.mouseButton.x;
		_mouseDownPosition.y = event.mouseButton.y;
		return this; 
	};
	onRightClick = onLeftClick;
	onLeftRelease = _nullAction_default;
	onRightRelease = _nullAction_default;
	onMiddleClick = _nullAction_default;
	onMiddleRelease = _nullAction_default;
	onScroll = [&, this](const sf::Event& event)
	{ 
		std::cout<< "delta :"<< event.mouseWheelScroll.delta << std::endl;
		this->Zoom( event.mouseWheelScroll.delta );
		return this; 
	};

	// passive mouse events
	whileLeftPressed = [&, this](const sf::Window& window)
	{
		sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
		glm::vec3 drag = glm::vec3(
			mousePosition.y-_mouseDownPosition.y,
			mousePosition.x-_mouseDownPosition.x,
			0.0f);
		GLfloat angle = glm::length(drag)==0?0:-0.01;

		if( drag.x!=0 && drag.y!=0 )
			this->Rotate( drag, angle, _position+ _lookAt );
		return this;
	};
	whileRightPressed = [&, this](const sf::Window& window)
	{
		sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
		sf::Vector2i drag = mousePosition-_mouseDownPosition;

		if( drag.x!=0 && drag.y!=0 )
			this->Move( glm::normalize( glm::vec3(-drag.x, -drag.y, 0.0)), true );
		return this;
	};
	whileLeftReleased = _nullState_default;
	whileRightReleased = _nullState_default;
	whileMiddlePressed = _nullState_default;
	whileMiddleReleased = _nullState_default;

	CalculateProjection();

	Use();
}

Camera::~Camera( void )
{
}


	// Button Callbacks ************************************************************************************


Camera* Camera::HandleActiveEvent( sf::Event& event)
{

	switch (event.type)
	{
		// mouse button pressed
		case sf::Event::MouseButtonPressed:
		{
			if (event.mouseButton.button == sf::Mouse::Left)
				this->onLeftClick(event);
			if (event.mouseButton.button == sf::Mouse::Right)
				this->onRightClick(event);
		} break;

		// mouse button released
		case sf::Event::MouseButtonReleased:
		{
			if (event.mouseButton.button == sf::Mouse::Left)
				this->onLeftRelease(event);
			if (event.mouseButton.button == sf::Mouse::Right)
				this->onRightRelease(event);
		} break;

		case sf::Event::MouseWheelScrolled:
		{
			if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
				this->onScroll(event);

		} break;

		default:
		break;

	}

	return this;
}


Camera* Camera::HandlePassiveState( sf::Window& window)
{
	// continued press
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) ) this->whileLeftPressed( window ); 
	else this->whileLeftReleased( window );

	if (sf::Mouse::isButtonPressed(sf::Mouse::Right) ) this->whileRightPressed( window ); 
	else this->whileRightReleased( window );

	if (sf::Mouse::isButtonPressed(sf::Mouse::Middle) ) this->whileMiddlePressed( window ); 
	else this->whileMiddleReleased( window );

	return this;

}


	// Camera Manipulation ************************************************************************************


Camera* Camera::Use( void )
{
	_currentCam = this;
	return this;
}

Camera* Camera::SetMode( enum CameraMode mode)
{
	_camMode = mode;
	return this;
}

Camera* Camera::Move( glm::vec3 displacement, bool stayFocused )
{
	_position += displacement;
	if(!stayFocused) _lookAt += displacement;
	return this;
}

Camera* Camera::MoveTo( glm::vec3 newPosition, bool stayFocused )
{
	glm::vec3 displacement = _position-newPosition;
	_position = newPosition;
	if(!stayFocused) _lookAt += displacement;
	return this;
}

Camera* Camera::SetCoordinates( enum CameraMode mode )
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

Camera* Camera::FocusOn( glm::vec3 target )
{
	_lookAt = target;		
	return this;
}


void Camera::CalculateProjection( void )
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

glm::mat4 Camera::GetProjectionView( void )
{

	try 
	{
		_view = glm::lookAt( 
				_position, // = eye isNan
				_lookAt,// = center
				glm::vec3(0.0, 0.1, 0.0) //up
				);
	} catch( std::exception& err)
	{
		std::cerr<< "eye: "<<glm::to_string(_position)<<"\ncenter:  "<<glm::to_string(_lookAt)<<std::endl;
	}

	return _projection * _view;
}

Camera* Camera::GetActiveCamera()
{
	return _currentCam ;
}


Camera* Camera::Rotate( glm::vec3 axis, GLfloat angle, glm::vec3 pivot )
{
	pivot = glm::normalize( pivot );

	glm::mat4 rotation = glm::rotate( glm::mat4(1.0f), angle, glm::normalize(axis) );
		// _position = ( rotation * glm::vec4( _position-pivot, 1.0) + glm::vec4(pivot,1.0) ).xyz();
	_position = glm::vec3( rotation * glm::vec4( _position-pivot, 1.0) + glm::vec4(pivot,1.0) );

	return this;
}


Camera* Camera::Zoom( GLfloat qtty )
{
	_fov += qtty;
	CalculateProjection();
	return this;
}


// Camera* Camera::AttachToObject(Object* o, vec3 offset=vec3(0.0f) )
// {
// 	o->AttachCamera( this, offset );
// }
