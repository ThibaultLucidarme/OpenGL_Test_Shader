
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include "Mesh.hpp"
#include "ShaderProgram.hpp"

using namespace std;





int main () {

	unsigned int size = 500;

	sf::ContextSettings glSettings;
	glSettings.majorVersion = 4;
	glSettings.minorVersion = 4;

    sf::RenderWindow window(sf::VideoMode(size, size), 
    						"Viewer3d -- SFML / OpenGL Shader", 
    						sf::Style::Titlebar|sf::Style::Resize|sf::Style::Close,
    						glSettings );

    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);
    
	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	glEnable (GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc (GL_LESS); // depth-testing interprets a smaller value as "closer"



	GLfloat matrix[] = {
		1.0f, 0.0f, 0.0f, 0.0f, // first column
		0.0f, 1.0f, 0.0f, 0.0f, // second column
		0.0f, 0.0f, 1.0f, 0.0f, // third column
		0.5f, 0.0f, 0.0f, 1.0f // fourth column
	};
	
	
	Mesh* mesh = new Mesh();


	ShaderProgram* prgrm = new ShaderProgram();
	prgrm->AddVertexShader( "../src/test_vs.glsl" )
		 ->AddFragmentShader( "../src/test_fs.glsl" )
		 ->SetParameter("matrix", matrix);
		

	glEnable( GL_CULL_FACE );
	glCullFace( GL_BACK );
	glFrontFace( GL_CW ); // GL_CCW for counter clock-wise

	bool running = true;
    while (running)
    {
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

		// wipe the drawing surface clear
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		// draw mesh
		prgrm->Begin();
		mesh->Draw();
		prgrm->End();

		// update buffer
		window.display();


	}


	delete prgrm;
	delete mesh;
	
	return 0;
}
