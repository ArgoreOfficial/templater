#include "cSceneGame.h"

#include <Core/cApplication.h>
#include <Core/Renderer/Framework/cVertexLayout.h>
#include <Core/cWindow.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ctime>
#include <string>

#define RANDF( _min, _max ) _min + (float)( rand() ) / ( (float)( RAND_MAX / ( _max - _min ) ) )

/* B-V to RGB */
void bv2rgb( double& r, double& g, double& b, double bv )    // RGB <0,1> <- BV <-0.4,+2.0> [-]
{
	double t;  r = 0.0; g = 0.0; b = 0.0; if ( bv < -0.4 ) bv = -0.4; if ( bv > 2.0 ) bv = 2.0;
	if ( ( bv >= -0.40 ) && ( bv < 0.00 ) ) { t = ( bv + 0.40 ) / ( 0.00 + 0.40 ); r = 0.61 + ( 0.11 * t ) + ( 0.1 * t * t ); }
	else if ( ( bv >= 0.00 ) && ( bv < 0.40 ) ) { t = ( bv - 0.00 ) / ( 0.40 - 0.00 ); r = 0.83 + ( 0.17 * t ); }
	else if ( ( bv >= 0.40 ) && ( bv < 2.10 ) ) { t = ( bv - 0.40 ) / ( 2.10 - 0.40 ); r = 1.00; }
	if ( ( bv >= -0.40 ) && ( bv < 0.00 ) ) { t = ( bv + 0.40 ) / ( 0.00 + 0.40 ); g = 0.70 + ( 0.07 * t ) + ( 0.1 * t * t ); }
	else if ( ( bv >= 0.00 ) && ( bv < 0.40 ) ) { t = ( bv - 0.00 ) / ( 0.40 - 0.00 ); g = 0.87 + ( 0.11 * t ); }
	else if ( ( bv >= 0.40 ) && ( bv < 1.60 ) ) { t = ( bv - 0.40 ) / ( 1.60 - 0.40 ); g = 0.98 - ( 0.16 * t ); }
	else if ( ( bv >= 1.60 ) && ( bv < 2.00 ) ) { t = ( bv - 1.60 ) / ( 2.00 - 1.60 ); g = 0.82 - ( 0.5 * t * t ); }
	if ( ( bv >= -0.40 ) && ( bv < 0.40 ) ) { t = ( bv + 0.40 ) / ( 0.40 + 0.40 ); b = 1.00; }
	else if ( ( bv >= 0.40 ) && ( bv < 1.50 ) ) { t = ( bv - 0.40 ) / ( 1.50 - 0.40 ); b = 1.00 - ( 0.47 * t ) + ( 0.1 * t * t ); }
	else if ( ( bv >= 1.50 ) && ( bv < 1.94 ) ) { t = ( bv - 1.50 ) / ( 1.94 - 1.50 ); b = 0.63 - ( 0.6 * t * t ); }
}

void cSceneGame::create( void )
{
	srand( time( 0 ) );

	cApplication& app = cApplication::getInstance();
	m_renderer = app.getRenderer();
	m_backend = m_renderer->getBackend();

	{ /* create particles */
		
		const double mindist = 4.7302642e13; // 5 light years
		const double maxdist = 1.89210568e14; // 15 light years

		for ( int i = 0; i < 1000; i++ )
		{
			double x = RANDF( mindist, maxdist );
			double y = RANDF( mindist, maxdist );
			double z = RANDF( mindist, maxdist );

			double vel_x = RANDF( 5.43e3, 9.68e3 );
			double vel_y = RANDF( 5.43e3, 9.68e3 );
			double vel_z = RANDF( 5.43e3, 9.68e3 );

			double mass = RANDF( 1.5911e29, 2.98365e32 );

			m_points.push_back( { { x, y, z }, { vel_x, vel_y, vel_z }, mass, { 1.0, 0.0, 1.0 } } );
		}

	}

	std::string frag = app.loadShaderSource( "../res/basic.frag" );
	std::string vert = app.loadShaderSource( "../res/basic.vert" );
	sShader frag_shader = m_renderer->createShader( frag.data(), eShaderType::Shader_Fragment );
	sShader vert_shader = m_renderer->createShader( vert.data(), eShaderType::Shader_Vertex );

	m_shader = m_backend->createShaderProgram( frag_shader, vert_shader );

	/* create vertex array */
	m_vertex_array = m_backend->createVertexArray();
	m_backend->bindVertexArray( m_vertex_array );

	/* create vertex buffer */
	m_vertex_buffer = m_backend->createBuffer( eBufferType::Buffer_Vertex );
	m_backend->bufferData( m_vertex_buffer, m_points.data(), m_points.size() * sizeof( sPoint ) );

	cVertexLayout layout;
	layout.push<double>( 3 );
	layout.push<double>( 3 );
	layout.push<double>( 1 );
	layout.push<double>( 3 );

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
	const double dt = 86400.0;
	const int integration_count = 1;

	for ( int integration = 0; integration < integration_count; integration++ )
	{
		for ( int i = 1; i < m_points.size() - 1; i++ )
		{
			cVector3f accumulated;

			for ( int other_index = 0; other_index < m_points.size(); other_index++ )
			{
				if ( other_index == i )
					continue;

				cVector3f rel = m_points[ i ].position - m_points[ other_index ].position;

				double dist_sqr = ( rel.x * rel.x + rel.y * rel.y + rel.z * rel.z );

				double acceleration = -1.0 * BIG_G * ( m_points[ other_index ].mass ) / dist_sqr; // pow( dist, 2.0 );

				accumulated += ( rel / sqrt( dist_sqr ) ) * acceleration;
			}

			m_points[ i ].velocity += accumulated * dt;
		}

		for ( int i = 1; i < m_points.size(); i++ )
		{
			m_points[ i ].position += m_points[ i ].velocity * dt;
		}
	}

	//printf( "frame\n" );
	/* buffer new data */
	m_backend->bufferData( m_vertex_buffer, m_points.data(), m_points.size() * sizeof( sPoint ) );
}

void cSceneGame::draw( void )
{
	cWindow& window = *cApplication::getInstance().getWindow();

	float aspect = window.getAspect();

	/* create transforms */

	glm::mat4 model = glm::mat4( 1.0f );
	model *= glm::scale( model, glm::vec3{ 0.0000000000001, 0.0000000000001, 0.0000000000001 } );

	glm::mat4 view = glm::mat4( 1.0f );
	view = glm::lookAt( glm::vec3{ 30.0f, 30.0f, 30.0f }, glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 0.0f, 1.0f, 0.0f } );

	glm::mat4 projection = glm::mat4( 1.0f );
	projection = glm::perspective( glm::radians( 45.0f ), aspect, 0.1f, 100.0f );


	/* apply transform uniforms */

	m_backend->useShaderProgram( m_shader );
	m_backend->setUniformMat4f( m_model_location, glm::value_ptr( model ) );
	m_backend->setUniformMat4f( m_view_location, glm::value_ptr( view ) );
	m_backend->setUniformMat4f( m_proj_location, glm::value_ptr( projection ) );

	/* draw call */

	m_backend->bindVertexArray( m_vertex_array );
	m_backend->drawArrays( m_points.size() );
	m_backend->bindVertexArray( 0 );

}
