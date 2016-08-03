#ifndef __INPUTDEVICE__
#define __INPUTDEVICE__

#include <SFML/Graphics.hpp>

// enum DeviceType
// {
// 	MOUSE,
// 	KEYBOARD
// };

class InputDevice
{
public:
	virtual InputDevice* HandleActiveEvent( sf::Event& ) = 0;
	virtual InputDevice* HandlePassiveState( sf::Window& ) = 0;

	virtual ~InputDevice( void ) {};
};


#endif