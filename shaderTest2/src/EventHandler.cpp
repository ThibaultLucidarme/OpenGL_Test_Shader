#include "EventHandler.hpp"
#include "Camera.hpp"


EventHandler::EventHandler( void )
{
	_close = false;
}

EventHandler::~EventHandler( void )
{
	_camera = NULL;
}

void EventHandler::RegisterDevice( InputDevice* device )
{
	if( dynamic_cast<Camera*>(device) ) _camera = device;
	// else if( dynamic_cast<Keyboard*>(device) ) _keyboard = device;
}

void EventHandler::HandleActiveEvent( sf::Event& event )
{

	if (event.type == sf::Event::Closed ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) ||
		sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
		_close = true;
			
	else if (event.type == sf::Event::Resized)
		glViewport(0, 0, event.size.width, event.size.height);	

    else if( event.type == sf::Event::KeyPressed ||
    		event.type == sf::Event::KeyReleased )
    	{/* _keyboard -> HandleActiveEvent( event );*/}

    else if( event.type == sf::Event::MouseButtonPressed ||
    		event.type == sf::Event::MouseButtonReleased ||
    		event.type == sf::Event::MouseWheelMoved ||
    		event.type == sf::Event::MouseMoved )
    	_camera -> HandleActiveEvent( event );

}

bool EventHandler::CloseWindow( void )
{
	return _close;
}

void EventHandler::HandlePassiveState( sf::Window& window )
{
	_camera -> HandlePassiveState( window );
} 
