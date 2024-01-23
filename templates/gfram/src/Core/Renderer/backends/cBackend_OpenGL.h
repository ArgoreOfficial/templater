#pragma once

#include <Core/renderer/backends/iBackend.h>

class cBackend_OpenGL : public iBackend
{
public:
	 cBackend_OpenGL();
	~cBackend_OpenGL();

	void create( cWindow& _window ) override;
	void clear( unsigned int _color ) override;
	
	iShader* createShader( const char* _source, eShaderType _type ) override;
	unsigned int createShaderProgram( iShader* _vertex_shader, iShader* _fragment_shader ) override;
private:

};
