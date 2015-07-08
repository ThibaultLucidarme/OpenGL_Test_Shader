/******************************************************************************\
| OpenGL 4 Example Code.                                                       |
| Accompanies written series "Anton's OpenGL 4 Tutorials"                      |
| Email: anton at antongerdelan dot net                                        |
| First version 27 Jan 2014                                                    |
| Copyright Dr Anton Gerdelan, Trinity College Dublin, Ireland.                |
| See individual libraries for separate legal notices                          |
|******************************************************************************|
| Matrices and Vectors                                                         |
| Note: code discussed in previous tutorials is moved into gl_utils file       |
| On Apple don't forget to uncomment the version number hint in start_gl()     |
\******************************************************************************/
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>
#define GL_LOG_FILE "gl.log"


bool parse_file_into_str (
	const char* file_name, char* shader_str, int max_len
) {
	shader_str[0] = '\0'; // reset string
	FILE* file = fopen (file_name , "r");
	if (!file) {
		//gl_log_err ("ERROR: opening file for reading: %s\n", file_name);
		return false;
	}
	int current_len = 0;
	char line[2048];
	strcpy (line, ""); // remember to clean up before using for first time!
	while (!feof (file)) {
		if (NULL != fgets (line, 2048, file)) {
			current_len += strlen (line); // +1 for \n at end
			if (current_len >= max_len) {
				// (
				//	"ERROR: shader length is longer than string buffer length %i\n",
				//	max_len
				//);
			}
			strcat (shader_str, line);
		}
	}
	if (EOF == fclose (file)) { // probably unnecesssary validation
		//gl_log_err ("ERROR: closing file from reading %s\n", file_name);
		return false;
	}
	return true;
}



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

	/* OTHER STUFF GOES HERE NEXT */
	GLfloat points[] = {
		 0.0f,	0.5f,	0.0f,
		 0.5f, -0.5f,	0.0f,
		-0.5f, -0.5f,	0.0f
	};
	
	GLfloat colours[] = {
		1.0f, 0.0f,  0.0f,
		0.0f, 1.0f,  0.0f,
		0.0f, 0.0f,  1.0f
	};
	
	GLuint points_vbo;
	glGenBuffers (1, &points_vbo);
	glBindBuffer (GL_ARRAY_BUFFER, points_vbo);
	glBufferData (GL_ARRAY_BUFFER, 9 * sizeof (GLfloat), points, GL_STATIC_DRAW);
	
	GLuint colours_vbo;
	glGenBuffers (1, &colours_vbo);
	glBindBuffer (GL_ARRAY_BUFFER, colours_vbo);
	glBufferData (GL_ARRAY_BUFFER, 9 * sizeof (GLfloat), colours, GL_STATIC_DRAW);
	
	GLuint vao;
	glGenVertexArrays (1, &vao);
	glBindVertexArray (vao);
	glBindBuffer (GL_ARRAY_BUFFER, points_vbo);
	glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer (GL_ARRAY_BUFFER, colours_vbo);
	glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray (0);
	glEnableVertexAttribArray (1);
	
	char vertex_shader[1024 * 256];
	char fragment_shader[1024 * 256];
	assert (parse_file_into_str ("../src/test_vs.glsl", vertex_shader, 1024 * 256));
	assert (parse_file_into_str ("../src/test_fs.glsl", fragment_shader, 1024 * 256));
	
	GLuint vs = glCreateShader (GL_VERTEX_SHADER);
	const GLchar* p = (const GLchar*)vertex_shader;
	glShaderSource (vs, 1, &p, NULL);
	glCompileShader (vs);
	
	// check for compile errors
	int params = -1;
	glGetShaderiv (vs, GL_COMPILE_STATUS, &params);
	if (GL_TRUE != params) {
		fprintf (stderr, "ERROR: GL shader index %i did not compile\n", vs);
		//print_shader_info_log (vs);
		return 1; // or exit or something
	}
	
	GLuint fs = glCreateShader (GL_FRAGMENT_SHADER);
	p = (const GLchar*)fragment_shader;
	glShaderSource (fs, 1, &p, NULL);
	glCompileShader (fs);
	
	// check for compile errors
	glGetShaderiv (fs, GL_COMPILE_STATUS, &params);
	if (GL_TRUE != params) {
		fprintf (stderr, "ERROR: GL shader index %i did not compile\n", fs);
		//print_shader_info_log (fs);
		return 1; // or exit or something
	}
	
	GLuint shader_programme = glCreateProgram ();
	glAttachShader (shader_programme, fs);
	glAttachShader (shader_programme, vs);
	glLinkProgram (shader_programme);
	
	glGetProgramiv (shader_programme, GL_LINK_STATUS, &params);
	if (GL_TRUE != params) {
		fprintf (
			stderr,
			"ERROR: could not link shader programme GL index %i\n",
			shader_programme
		);
		//print_programme_info_log (shader_programme);
		return false;
	}
	
	GLfloat matrix[] = {
		1.0f, 0.0f, 0.0f, 0.0f, // first column
		0.0f, 1.0f, 0.0f, 0.0f, // second column
		0.0f, 0.0f, 1.0f, 0.0f, // third column
		0.5f, 0.0f, 0.0f, 1.0f // fourth column
	};
	

	int matrix_location = glGetUniformLocation (shader_programme, "matrix");
	glUseProgram (shader_programme);
	glUniformMatrix4fv (matrix_location, 1, GL_FALSE, matrix);
	
	glEnable (GL_CULL_FACE); // cull face
	glCullFace (GL_BACK); // cull back face
	glFrontFace (GL_CW); // GL_CCW for counter clock-wise

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
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//
		// Note: this call is not necessary, but I like to do it anyway before any
		// time that I call glDrawArrays() so I never use the wrong shader programme
		glUseProgram (shader_programme);
		
		
		//
		// Note: this call is related to the most recently 'used' shader programme
		glUniformMatrix4fv (matrix_location, 1, GL_FALSE, matrix);
		
		//
		// Note: this call is not necessary, but I like to do it anyway before any
		// time that I call glDrawArrays() so I never use the wrong vertex data
		glBindVertexArray (vao);
		// draw points 0-3 from the currently bound VAO with current in-use shader
		glDrawArrays (GL_TRIANGLES, 0, 3);
		// put the stuff we've been drawing onto the display
		window.display();
	}
	
	return 0;
}
