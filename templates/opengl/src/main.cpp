#include <stdio.h>

#include "rendering/cWindow.h"
#include "rendering/cRenderer.h"

int main()
{
	cWindow window;
	cRenderer renderer;

	window.create( 800, 600, "OpenGL.exe" );
	renderer.create( window );

	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};

	unsigned int vao;
	glGenVertexArrays( 1, &vao );
	glBindVertexArray( vao );

	unsigned int vbo;
	glGenBuffers( 1, &vbo );
	glBindBuffer( GL_ARRAY_BUFFER, vbo );
	glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );

	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof( float ), (void*)0 );
	glEnableVertexAttribArray( 0 );

	while ( !window.shouldClose() )
	{
		window.startFrame();
		renderer.clear( 0x000000FF );

		glUseProgram( renderer.getDefaultShader() );
		glBindVertexArray( vao );
		glDrawArrays( GL_TRIANGLES, 0, 3 );

		window.endFrame();
	}

	glfwTerminate();
}
