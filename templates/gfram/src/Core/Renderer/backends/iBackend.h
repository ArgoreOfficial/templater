#pragma once

class cWindow;
class iShader;
class cShaderProgram;
enum eShaderType;

class iBackend
{
public:
	virtual ~iBackend() { }

	virtual void create ( cWindow& _window ) { }
	virtual void clear  ( unsigned int _color ) { }
	virtual void destroy( void ) { }

	virtual void onResize( void ) { }

	virtual void beginFrame( void ) { }
	virtual void endFrame  ( void ) { }

	virtual iShader* createShader( const char* _source, eShaderType _type ) { return nullptr; }
	virtual unsigned int createShaderProgram( iShader* _vertex_shader, iShader* _fragment_shader ) { return 0; }

protected:
	iBackend() { }

private:

};
