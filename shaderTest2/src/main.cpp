
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include <iostream>

#include "Mesh.hpp"
#include "ShaderProgram.hpp"

using namespace std;





int main () {

	unsigned int size = 500;



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
	

	glm::mat4 Projection = glm::perspective( 90.0f, //fov
											4.0f / 3.0f, //aspect ratio
											0.1f, //near plane
											100.f //far plane
										);

	glm::mat4 View 		 = glm::lookAt( glm::vec3(1.0, 1.0, 1.0), //eye
										glm::vec3(0.0, 0.0, 0.0), //center
										glm::vec3(0.0, 0.1, 0.0) //up
									);

	ShaderProgram* prgrm = new ShaderProgram();
	prgrm->AddVertexShader( "../src/test_vs.glsl" )
	->AddFragmentShader( "../src/test_fs.glsl" )
	->SetParameter( "ProjectionViewMatrix", Projection*View );


	// Load Object ************************************************************************************



	Object* s1 = new Object( "../data/suzanne.obj" );
	

	// ************************************************************************************

	prgrm->Begin();

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

		}

	// Draw ************************************************************************************


		s1->Move( 0.01 )
		  ->Draw( GL_TRIANGLES );

		// update buffer
		window.display();


	}

	prgrm->End();
	

	delete prgrm;
	delete s1;

	return EXIT_SUCCESS;
}
