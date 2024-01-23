#include "cBackend_OpenGL.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h> /* temporary */

#include <Core/cWindow.h>

#include <stdio.h>

#include <Core/Renderer/Shader/iShader.h>
#include <Core/Renderer/Shader/cShader_OpenGL.h>
#include <Core/Renderer/Shader/cShaderProgram.h>

cBackend_OpenGL::cBackend_OpenGL()
{

}

cBackend_OpenGL::~cBackend_OpenGL()
{

}

void cBackend_OpenGL::create( cWindow& _window )
{
	if ( !gladLoadGLLoader( (GLADloadproc)glfwGetProcAddress ) )
	{
		fprintf( stderr, "Failed to initialize GLAD\n" );
		return;
	}
	glViewport( 0, 0, _window.getWidth(), _window.getHeight() );

}

void cBackend_OpenGL::clear( unsigned int _color )
{
	float r = ( _color & 0xFF000000 ) / 256.0f;
	float g = ( _color & 0x00FF0000 ) / 256.0f;
	float b = ( _color & 0x0000FF00 ) / 256.0f;
	float a = ( _color & 0x000000FF ) / 256.0f;

	glClearColor( r, g, b, a );
	glClear( GL_COLOR_BUFFER_BIT );
}

iShader* cBackend_OpenGL::createShader( const char* _source, eShaderType _type )
{
	int  success;
	char infoLog[ 512 ];
	unsigned int shader;

	switch ( _type )
	{
	case Vertex:
		shader = glCreateShader( GL_VERTEX_SHADER );
		break;
	case Fragment:
		shader = glCreateShader( GL_FRAGMENT_SHADER );
		break;
	default:
		return nullptr;
		break;
	}

	glShaderSource( shader, 1, &_source, NULL );
	glCompileShader( shader );

	glGetShaderiv( shader, GL_COMPILE_STATUS, &success );
	if ( !success )
	{
		glGetShaderInfoLog( shader, 512, NULL, infoLog );
		printf( "ERROR::SHADER::COMPILATION_FAILED\n %s \n", infoLog );
	}

	cShader_OpenGL* shader_object = new cShader_OpenGL( shader );

	return shader_object;
}

unsigned int cBackend_OpenGL::createShaderProgram( iShader* _vertex_shader, iShader* _fragment_shader )
{
	int  success;
	char infoLog[ 512 ];
	unsigned int program = glCreateProgram();

	unsigned int vert = *(unsigned int*)( _vertex_shader->getHandle() ); /* cursed 23:52 code */
	unsigned int frag = *(unsigned int*)( _fragment_shader->getHandle() );

	glAttachShader( program, vert );
	glAttachShader( program, frag );
	glLinkProgram( program );

	glGetProgramiv( program, GL_LINK_STATUS, &success );
	if ( !success )
	{
		glGetProgramInfoLog( program, 512, NULL, infoLog );
		printf( "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n %s \n", infoLog );
	}

	glDeleteShader( vert );
	glDeleteShader( frag );

	return program;
}
