
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include <iostream>

#include "CommandLineParser.hpp"
#include "Camera.hpp"
#include "Mesh.hpp"
#include "ShaderProgram.hpp"

using namespace std;



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
	
	sf::Vector2i mouseDownPosition, mousePosition;


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
	objectPrgrm->AddVertexShader( "../src/test_vs.glsl" )
	->AddFragmentShader( "../src/test_fs.glsl" );

	/*
	ShaderProgram* terrainPrgrm = new ShaderProgram();
	terrainPrgrm->AddVertexShader( "../src/test_vs.glsl" )
			    ->AddFragmentShader( "../src/test_fs.glsl" );
	if(tesselate) terrainPrgrm->AddTesselationShaders("../src/test_fs.glsl", "../src/test_fs.glsl");
	//*/
	
	
	// objectPrgrm->SetParameter( "ProjectionViewMatrix", Camera::GetActiveCamera()->GetProjectionView() );// todo, eventually have Camera automatically/internally set CurrentProgram ProjectionViewMatrix parameter
	// objectPrgrm->SetParameter( "ProjectionViewMatrix", Projection*View );// todo, eventually have Camera automatically/internally set CurrentProgram ProjectionViewMatrix parameter
	objectPrgrm->SetParameter( "ProjectionViewMatrix", camera->GetProjectionView() );// todo, eventually have Camera automatically/internally set CurrentProgram ProjectionViewMatrix parameter


	// Load Object ************************************************************************************



	
	objectPrgrm->Begin();
	Object* s1 = new Object( modelFilename , 01);

	// ************************************************************************************


	bool running = true;
	while (running)
	{
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );


	// Event ************************************************************************************

		sf::Event event;
		while (window.pollEvent(event))
		{

			if (event.type == sf::Event::Closed ||
				sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) ||
				sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
				running = false;
			
			else if (event.type == sf::Event::Resized)
				glViewport(0, 0, event.size.width, event.size.height);	

			else if( event.type == sf::Event::MouseButtonPressed &&
                     event.mouseButton.button == sf::Mouse::Left ) 
            {
                mouseDownPosition.x = event.mouseButton.x;
                mouseDownPosition.y = event.mouseButton.y;
            }		

		}

	// Draw ************************************************************************************

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) )
		{
			mousePosition = sf::Mouse::getPosition(window);
			glm::vec3 drag = glm::vec3(
				mousePosition.y-mouseDownPosition.y,
				mousePosition.x-mouseDownPosition.x,
				 0.0f);
			GLfloat angle = glm::length(drag)==0?0:-0.01;

			if( drag.x==0 && drag.y==0 ) drag = glm::vec3(0.0, 1.0, 0.0);

			camera->Rotate( drag, angle, s1->GetPosition() );
			objectPrgrm->SetParameter( "ProjectionViewMatrix", camera->GetProjectionView() );

		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Right) )
		{
			mousePosition = sf::Mouse::getPosition(window);
			sf::Vector2i drag = mousePosition-mouseDownPosition;

			camera->Move( glm::normalize( glm::vec3(-drag.x, -drag.y, 0.0)) );
			objectPrgrm->SetParameter( "ProjectionViewMatrix", camera->GetProjectionView() );

		}

		s1->Move( 0.01 );
		s1->Draw( GL_POINTS );

		// update buffer
		window.display();


	}

	objectPrgrm->End();
	

	delete objectPrgrm;
	delete camera;
	delete s1;

	return EXIT_SUCCESS;
}
