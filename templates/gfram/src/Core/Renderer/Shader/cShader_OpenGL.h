#pragma once

#include "iShader.h"

class cShader_OpenGL : public iShader
{
public:
	cShader_OpenGL( unsigned int _handle ) :
		m_handle{ _handle }
	{ }
    ~cShader_OpenGL( void ) { }

	void* getHandle() override { return &m_handle; }

private:

	unsigned int m_handle;

};
