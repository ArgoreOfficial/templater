#include "cRenderer.h"

#include <Core/Renderer/backends/cBackend_OpenGL.h>
#include <Core/Renderer/backends/cBackend_D3D11.h>
#include <Core/Renderer/Shader/iShader.h>
cRenderer::cRenderer( void ):
	m_backend{ nullptr }
{

}

cRenderer::~cRenderer( void )
{

}

void cRenderer::create( cWindow& _window, cRenderer::eBackendType _backend )
{
	switch ( _backend )
	{
	case OpenGL:
		m_backend = new cBackend_OpenGL();
		break;
	case Direct3D11:
		m_backend = new cBackend_D3D11();
		break;
	default:
		m_backend = new cBackend_OpenGL();
	}

	m_backend->create( _window );

	createDefaultShader();
}

void cRenderer::clear( unsigned int _color )
{
	m_backend->clear( _color );
}

void cRenderer::beginFrame( void )
{
	m_backend->beginFrame();
}

void cRenderer::endFrame( void )
{
	m_backend->endFrame();
}

void cRenderer::drawPointCloud( cPointCloud& _point_cloud )
{
	
}

void cRenderer::createDefaultShader( void )
{
	iShader* vertex_shader = m_backend->createShader( m_default_vertex_shader_source, eShaderType::Vertex );
	iShader* fragment_shader = m_backend->createShader( m_default_fragment_shader_source, eShaderType::Fragment );
	m_shader_default = m_backend->createShaderProgram( vertex_shader, fragment_shader );
}
