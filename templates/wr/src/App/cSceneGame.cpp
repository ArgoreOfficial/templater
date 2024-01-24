#include "cSceneGame.h"

#include <Core/cApplication.h>
#include <Core/Renderer/Framework/cVertexLayout.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void cSceneGame::create( void )
{
	cApplication& app = cApplication::getInstance();
	m_renderer = app.getRenderer();
	m_backend = m_renderer->getBackend();

	m_points.push_back( { -0.5f, -0.5f, 0.0f } );
	m_points.push_back( { 0.5f, -0.5f, 0.0f } );
	m_points.push_back( { 0.0f,  0.5f, 0.0f } );

	m_shader = m_renderer->getDefaultShader();

	/* create vertex array */
	m_vertex_array = m_backend->createVertexArray();
	m_backend->bindVertexArray( m_vertex_array );

	/* create vertex buffer */
	m_vertex_buffer = m_backend->createBuffer( eBufferType::Buffer_Vertex );
	m_backend->bufferData( m_vertex_buffer, m_points.data(), m_points.size() * sizeof( sPoint ) );

	cVertexLayout layout;
	layout.push<float>( 3 );

	m_backend->bindVertexLayout( layout );

	m_model_location = m_backend->getUniformLocation( m_shader, "model" );
	m_view_location = m_backend->getUniformLocation( m_shader, "view" );
	m_proj_location = m_backend->getUniformLocation( m_shader, "proj" );

}

void cSceneGame::destroy( void )
{
}

void cSceneGame::update( double _delta_time )
{
	m_points[ 2 ].y += 0.01f;
}

void cSceneGame::draw( void )
{
	glm::mat4 model = glm::mat4( 1.0f );
	model = glm::rotate( model, glm::radians( -55.0f ), glm::vec3( 1.0f, 0.0f, 0.0f ) );

	glm::mat4 view = glm::mat4( 1.0f );
	view = glm::translate( view, glm::vec3( 0.0f, 0.0f, -3.0f ) );

	glm::mat4 projection = glm::mat4( 1.0f );
	projection = glm::perspective( glm::radians( 45.0f ), 800.0f / 600.0f, 0.1f, 100.0f );

	m_backend->bufferData( m_vertex_buffer, m_points.data(), m_points.size() * sizeof( sPoint ) );

	m_backend->useShaderProgram( m_shader );
	m_backend->setUniformMat4f( m_model_location, glm::value_ptr( model ) );
	m_backend->setUniformMat4f( m_view_location, glm::value_ptr( view ) );
	m_backend->setUniformMat4f( m_proj_location, glm::value_ptr( projection ) );

	m_backend->bindVertexArray( m_vertex_array );
	m_backend->drawArrays( m_points.size() );
	m_backend->bindVertexArray( 0 );

}
