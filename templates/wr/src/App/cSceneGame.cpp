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
#define RANDD( _min, _max ) _min + (double)( rand() ) / ( (double)( RAND_MAX / ( _max - _min ) ) )
#define BIG_G 6.67e-11

/* B-V to RGB */
void bv2rgb( double& r, double& g, double& b, double bv )    // RGB <0,1> <- BV <-0.4,+2.0> [-]
{
	double t;
	r = 0.0; g = 0.0; b = 0.0;
	if ( bv < -0.4 ) bv = -0.4;
	if ( bv > 2.0 ) bv = 2.0;
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

double mapRange( double _x, double _min_in, double _max_in, double _min_out, double _max_out )
{
	double x = ( _x - _min_in ) / ( _max_in - _min_in );
	return _min_out + ( _max_out - _min_out ) * x;
}

void cSceneGame::create( void )
{
	srand( time( 0 ) );

	cApplication& app = cApplication::getInstance();
	m_renderer = app.getRenderer();
	m_backend = m_renderer->getBackend();

	m_octree.create();

	{ /* create particles */

		const double max_dist = 4.7302642e17; // ~50,000 light years ( radius of milky way )
		const double max_speed = 9.68e28;
		const double min_mass = 1.5911e29;
		const double max_mass = 3.38147e32;

		const double black_hole_mass = 7.9564e36; /* roughly mass of Sagittarius A* */

		for ( int i = 0; i < 1000; i++ )
		{
			double t = RANDD( 0, 3.1415 * 2.0 );
			double dist = RANDD( max_dist * 0.2, max_dist );
			double s = sin( t ) * dist;
			double c = cos( t ) * dist;

			double mass = RANDF( min_mass, max_mass );
			double v = 0.0f;

			if( i > 0 )
				v = sqrt( BIG_G * black_hole_mass / dist ) * max_speed * 0.00005;
			
			double s_prim = cos( t );
			double c_prim = -sin( t );

			double x = s; // RANDF( -max_dist, max_dist );
			double y = RANDF( -max_dist * 0.2, max_dist * 0.2 );
			double z = c; // RANDF( -max_dist, max_dist );

			double vel_x = s_prim * v; // RANDF( -max_speed, max_speed );
			double vel_y = RANDF( -max_speed * 0.2, max_speed * 0.2 );
			double vel_z = c_prim * v; // RANDF( -max_speed, max_speed );

			double bv = mapRange( mass, min_mass, max_mass, 2.0, -0.4 );
			double l = mapRange( mass, min_mass, max_mass, 0.3, 1.0 );
			double r = 0, g = 0, b = 0;
			bv2rgb( r, g, b, bv );

			m_points.push_back( { { x, y, z }, { vel_x, vel_y, vel_z }, mass, cVector3f{ r, g, b } * l } );
		}

		m_points[ 0 ].position = { 0,0,0 };
		m_points[ 0 ].mass = black_hole_mass; 
		m_points[ 0 ].color = { 0.0, 1.0, 0.0 };
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

cVector3f computeForce( cVector3f& _1p, cVector3f& _2p, double& _1m, double& _2m )
{
	cVector3f rel = _1p - _2p;
	double dist = sqrt( rel.x * rel.x + rel.y * rel.y + rel.z * rel.z );

	cVector3f quotient = rel * ( BIG_G * _1m * _2m );
	return quotient / pow( dist, 2.0 );
}

void accumulateForces( sPoint& _point, std::vector<sPoint>& _points, double _delta_time )
{
	cVector3f f;

	for ( int j = 0; j < _points.size(); j++ )
	{
		if ( &_point == &_points[ j ] )
			continue;

		sPoint& m1 = _point;
		sPoint& m2 = _points[ j ];

		f -= computeForce( m1.position, m2.position, m1.mass, m2.mass );
	}

	_point.velocity += f * _delta_time;
}

void cSceneGame::update( double _delta_time )
{
	// const double dt = 3.155e8;
	const double dt = 0.000000000000005;
	m_points[ 0 ].position = { 0,0,0 };

	for ( int i = 0; i < m_points.size(); i++ )
	{
		accumulateForces( m_points[ i ], m_points, dt );
	}

	for ( int i = 0; i < m_points.size(); i++ )
	{
		m_points[ i ].position += m_points[ i ].velocity * dt;
	}
	m_points[ 0 ].position = { 0,0,0 };

	/* buffer new data */
	m_backend->bufferData( m_vertex_buffer, m_points.data(), m_points.size() * sizeof( sPoint ) );
}

void cSceneGame::draw( void )
{
	cWindow& window = *cApplication::getInstance().getWindow();

	const double scale = 0.00000000000000003;

	float aspect = window.getAspect();

	/* create transforms */

	glm::mat4 model = glm::mat4( 1.0f );
	model *= glm::scale( model, glm::vec3{ scale, scale, scale } );

	glm::mat4 view = glm::mat4( 1.0f );
	view = glm::lookAt( glm::vec3{ 25.0f, 15.0f, 30.0f }, glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 0.0f, 1.0f, 0.0f } );
	//view = glm::lookAt( glm::vec3{ 0.0f, 70.0f, 0.0f }, glm::vec3{ 0.0f, 0.0f, 0.0f }, glm::vec3{ 1.0f, 0.0f, 0.0f } );

	glm::mat4 projection = glm::mat4( 1.0f );
	projection = glm::perspective( glm::radians( 45.0f ), aspect, 0.1f, 100.0f );


	/* apply transform uniforms */

	m_backend->useShaderProgram( m_shader );
	m_backend->setUniformMat4f( m_model_location, glm::value_ptr( model ) );
	m_backend->setUniformMat4f( m_view_location, glm::value_ptr( view ) );
	m_backend->setUniformMat4f( m_proj_location, glm::value_ptr( projection ) );

	/* draw call */

	m_backend->bindVertexArray( m_vertex_array );
	m_backend->drawArrays( m_points.size(), eDrawMode::DrawMode_Points );
	m_backend->bindVertexArray( 0 );

	m_octree.drawNodeTree();

}
