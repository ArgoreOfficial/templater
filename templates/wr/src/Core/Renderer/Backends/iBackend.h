#pragma once

class cWindow;
class cVertexLayout;

#include <Core/Renderer/Framework/Shader.h>
#include <Core/Renderer/Framework/Buffer.h>
#include <Core/Renderer/Framework/VertexArray.h>

enum eDrawMode
{
	DrawMode_Points,
	DrawMode_Lines,
	DrawMode_Triangle
};

class iBackend
{
public:
	virtual ~iBackend() { }

	virtual void create ( cWindow& _window ) { }
	virtual void clear  ( unsigned int _color ) { }
	virtual void destroy( void ) { }

	virtual void onResize( int _width, int _height ) { }

	virtual void beginFrame( void ) { }
	virtual void endFrame  ( void ) { }

	virtual sShader createShader( const char* _source, eShaderType _type ) { return { 0, eShaderType::Shader_None }; }
	virtual hShaderProgram createShaderProgram( sShader& _vertex_shader, sShader& _fragment_shader ) { return 0; }
	virtual sBuffer createBuffer( eBufferType _type ) { return { 0, eBufferType::Buffer_None }; }
	virtual hVertexArray createVertexArray() { return 0; }

	virtual void bufferData( sBuffer& _buffer, void* _data, size_t _size ) { }

	virtual void useShaderProgram( hShaderProgram _program ) { }
	virtual void bindVertexLayout( cVertexLayout& _layout ) { }
	virtual void bindVertexArray( hVertexArray _vertex_array ) { }

	virtual void drawArrays( unsigned int _vertex_count, eDrawMode _mode ) { }
	virtual void drawElements( unsigned int _index_count, eDrawMode _mode ) { }

	virtual int getUniformLocation( hShaderProgram _shader, const char* _uniform ) { return -1; }
	virtual void setUniformMat4f( int _location, float* _matrix_ptr ) { }

protected:
	iBackend() { }

private:

};
