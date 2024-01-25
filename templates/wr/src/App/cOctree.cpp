#include "cOctree.h"

#include <Core/cApplication.h>
#include <Core/Renderer/Framework/cVertexLayout.h>
#include <Core/cWindow.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>

cOctree::cOctree()
{
}

cOctree::~cOctree()
{
}

void cOctree::create()
{
	cApplication& app = cApplication::getInstance();
	m_renderer = app.getRenderer();
	m_backend = m_renderer->getBackend();

	double vertices[] = {
		-0.5, 0.5,  0.5,     0.5, 0.5,  0.5, // top square
		 0.5, 0.5,  0.5,     0.5, 0.5, -0.5,
		 0.5, 0.5, -0.5,    -0.5, 0.5, -0.5,
		-0.5, 0.5, -0.5,    -0.5, 0.5,  0.5,

		-0.5, -0.5,  0.5,    0.5, -0.5,  0.5, // bottom square
		 0.5, -0.5,  0.5,    0.5, -0.5, -0.5,
		 0.5, -0.5, -0.5,   -0.5, -0.5, -0.5,
		-0.5, -0.5, -0.5,   -0.5, -0.5,  0.5,

		-0.5, -0.5,  0.5,   -0.5,  0.5,  0.5, // connecting edges
		 0.5, -0.5,  0.5,    0.5,  0.5,  0.5,
		 0.5, -0.5, -0.5,    0.5,  0.5, -0.5,
		-0.5, -0.5, -0.5,   -0.5,  0.5, -0.5
	};

	std::string frag = app.loadShaderSource( "../res/debug_lines.frag" );
	std::string vert = app.loadShaderSource( "../res/debug_lines.vert" );
	sShader frag_shader = m_renderer->createShader( frag.data(), eShaderType::Shader_Fragment );
	sShader vert_shader = m_renderer->createShader( vert.data(), eShaderType::Shader_Vertex );

	m_box_shader = m_backend->createShaderProgram( frag_shader, vert_shader );

	/* create vertex array */
	m_debug_box_vao = m_backend->createVertexArray();
	m_backend->bindVertexArray( m_debug_box_vao );

	/* create vertex buffer */
	m_debug_box_vbo = m_backend->createBuffer( eBufferType::Buffer_Vertex );
	m_backend->bufferData( m_debug_box_vbo, vertices, sizeof( vertices ) );

	cVertexLayout layout;
	layout.push<double>( 3 );

	m_backend->bindVertexLayout( layout );

	m_model_location = m_backend->getUniformLocation( m_box_shader, "model" );
	m_view_location = m_backend->getUniformLocation( m_box_shader, "view" );
	m_proj_location = m_backend->getUniformLocation( m_box_shader, "proj" );


}

void cOctree::drawNodeTree()
{
	cWindow& window = *cApplication::getInstance().getWindow();

	const double scale = 20.0;

	float aspect = window.getAspect();

	/* create transforms */

	glm::mat4 model = glm::mat4( 1.0f );
	model *= glm::scale( model, glm::vec3{ scale, scale * 0.2, scale } );

	glm::mat4 view = glm::mat4( 1.0f );
	view = glm::lookAt( glm::vec3{ 25.0f, 15.0f, 30.0f }, glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 0.0f, 1.0f, 0.0f } );
	//view = glm::lookAt( glm::vec3{ 0.0f, 70.0f, 0.0f }, glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 1.0f, 0.0f, 0.0f } );

	glm::mat4 projection = glm::mat4( 1.0f );
	projection = glm::perspective( glm::radians( 45.0f ), aspect, 0.1f, 100.0f );

	/* apply transform uniforms */

	m_backend->useShaderProgram( m_box_shader );
	m_backend->setUniformMat4f( m_model_location, glm::value_ptr( model ) );
	m_backend->setUniformMat4f( m_view_location, glm::value_ptr( view ) );
	m_backend->setUniformMat4f( m_proj_location, glm::value_ptr( projection ) );

	/* draw call */

	m_backend->bindVertexArray( m_debug_box_vao );
	m_backend->drawArrays( 72 / 3, eDrawMode::DrawMode_Lines );
	m_backend->bindVertexArray( 0 );
}
