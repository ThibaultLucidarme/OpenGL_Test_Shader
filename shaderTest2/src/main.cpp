
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include <iostream>

#include "CommandLineParser.hpp"
#include "Mesh.hpp"
#include "ShaderProgram.hpp"
#include "EventHandler.hpp"
#include "Camera.hpp"

using namespace std;

// DEBUG cout glm structs ***********************************************

#include <glm/gtx/string_cast.hpp>

// template<typename genType>
// std::ostream& operator<<(std::ostream& out, const genType& g)
// {
//     return out << glm::to_string(g);
// }
// **********************************************************************


int main( int argc, char** argv) {
	
	// Execution parameters ************************************************************************************
		
		
	p::CommandLineParser parser(argc, argv);

	int size = parser.addOption<int>("-s",500,"Window size");
	std::string modelFilename = parser.addOption<std::string>("-m","../data/suzanne.obj","Model Mesh OBJ file");
	//bool tesselate = (parser.addOption<int>("--tesselation",1, "Use Tesselation Shader ( 0 | 1 )")==1);
	
	parser.addHelpOption();

	
	// Init ************************************************************************************

	sf::ContextSettings glSettings;
	glSettings.majorVersion = 4;
	glSettings.minorVersion = 4;

	sf::RenderWindow window(sf::VideoMode(size, size), 
		"Viewer3d -- SFML / OpenGL Shader", 
		sf::Style::Titlebar|sf::Style::Resize|sf::Style::Close,
		glSettings );

	window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(60);
	window.setKeyRepeatEnabled(false);


	// OpenGL Options ************************************************************************************


	// depth
	glEnable( GL_DEPTH_TEST ); // enable depth-testing
	glDepthFunc (GL_LESS ); // depth-testing interprets a smaller value as "closer"
	
	// cull faces
	glEnable( GL_CULL_FACE );
	glCullFace( GL_BACK );
	glFrontFace( GL_CCW ); // GL_CCW for counter clock-wise
	
	// face order
	// glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );


	// View ************************************************************************************

	Camera* camera = new Camera( CAM_PERSP, glm::vec3(0.0, 0.0, 30.0) );


	ShaderProgram* objectPrgrm = new ShaderProgram();
	objectPrgrm->AddVertexShader( "../src/test.vs.glsl" )
	->AddFragmentShader( "../src/test.fs.glsl" )
	->SetParameter( "ProjectionViewMatrix", camera->GetProjectionView() )
	->Begin();

	// Load Object ************************************************************************************

	Object* s1 = new Object( modelFilename , 01);


	// ************************************************************************************

	EventHandler* eventHandler = new EventHandler();
	eventHandler->RegisterDevice( camera ); 
	// eventHandler->RegisterDevice( keyboard ); 

	while ( !eventHandler->CloseWindow() )
	{
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );


	// Event ************************************************************************************

		sf::Event event;

		while (window.pollEvent(event))
		{
			eventHandler->HandleActiveEvent( event );
		}

		eventHandler->HandlePassiveState( window );

		objectPrgrm->SetParameter( "ProjectionViewMatrix", camera->GetProjectionView() );

	// Draw ************************************************************************************

		s1->Move( 0.01 );
		s1->Draw( GL_POINTS );

		// update buffer
		window.display();


	}

	objectPrgrm->End();
	
	delete eventHandler;
	delete objectPrgrm;
	delete camera;
	delete s1;

	return EXIT_SUCCESS;
}
