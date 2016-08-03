#ifndef __EVENTHANDLER__
#define __EVENTHANDLER__

#include <SFML/Graphics.hpp>
#include "InputDevice.hpp"

class EventHandler
{
private:
	bool _close;
	InputDevice* _camera;
	//InputDevice* _keyboard;

public:
	EventHandler( void );
	~EventHandler();
	void RegisterDevice( InputDevice*);
	void HandleActiveEvent(sf::Event&);
	void HandlePassiveState( sf::Window& );
	bool CloseWindow( void );

};



#endif
