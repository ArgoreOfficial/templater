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
	srand( (int)time( 0 ) );

	cApplication& app = cApplication::getInstance();
	m_renderer = app.getRenderer();
	m_backend = m_renderer->getBackend();
	
	m_octree = new cOctree();
	m_octree->create( 1.0e21 );

	{ /* create particles */

		const double max_dist = 1.7302642e20;
		const double speed_deviation = 3.0e2;
		const double height = 4.0e18;
		const double min_mass = 1.5911e29;
		const double max_mass = 3.38147e32;

		const double black_hole_mass = 7.9564e36; /* roughly mass of Sagittarius A* */

		for ( int i = 0; i < 100; i++ )
		{
			double t = RANDD( 0, 3.1415 * 2.0 );
			double max_dist_temp = RANDD( 0, max_dist );
			double height_temp = RANDD( 0, height );
			double dist = RANDD( max_dist_temp * 0.01, max_dist_temp );
			double s = sin( t ) * dist;
			double c = cos( t ) * dist;

			double mass = RANDD( min_mass, max_mass );
			double v = 0.0f;

			if( i > 0 )
				v = sqrt( BIG_G * black_hole_mass / dist ) + RANDD( -speed_deviation, speed_deviation );
			
			double s_prim = cos( t );
			double c_prim = -sin( t );

			/* position */
			double x = s; // RANDF( -max_dist, max_dist );
			double y = RANDD( -height_temp, height_temp );
			double z = c; // RANDF( -max_dist, max_dist );

			/* initial velocity */
			double vel_x = s_prim * v; // RANDF( -max_speed, max_speed );
			double vel_y = 0.0; // RANDF( -max_speed * 0.2, max_speed * 0.2 );
			double vel_z = c_prim * v; // RANDF( -max_speed, max_speed );

			/* star colour */
			double bv = mapRange( mass, min_mass, max_mass, 2.0, -0.4 );
			double l = mapRange( mass, min_mass, max_mass, 0.3, 1.0 );
			double r = 0, g = 0, b = 0;
			bv2rgb( r, g, b, bv );

			sPoint point;
			point.position = { x, y, z };
			point.velocity = { vel_x, vel_y, vel_z };
			point.mass = mass;
			m_points.push_back( point );
			m_render_points.push_back( { m_points[ i ].position, wv::cVector4d{ r, g, b, l } });
		}

		m_points[ 0 ].position = { 1,1,1 };
		m_points[ 0 ].mass = black_hole_mass;
		m_render_points[ 0 ].position = m_points[ 0 ].position;
		m_render_points[ 0 ].color = { 0.0, 1.0, 0.0, 1.0 };
	}

	std::string vert = app.loadShaderSource( "../res/star.vert" );
	std::string geom = app.loadShaderSource( "../res/billboard.geom" );
	std::string frag = app.loadShaderSource( "../res/star.frag" );
	sShader vert_shader = m_renderer->createShader( vert.data(), eShaderType::Shader_Vertex );
	sShader geom_shader = m_renderer->createShader( geom.data(), eShaderType::Shader_Geometry );
	sShader frag_shader = m_renderer->createShader( frag.data(), eShaderType::Shader_Fragment );

	m_shader = m_backend->createShaderProgram();
	m_backend->attachShader( m_shader, vert_shader );
	//m_backend->attachShader( m_shader, geom_shader );
	m_backend->attachShader( m_shader, frag_shader );
	m_backend->linkShaderProgram( m_shader );

	/* create vertex array */
	m_vertex_array = m_backend->createVertexArray();
	m_backend->bindVertexArray( m_vertex_array );

	/* create vertex buffer */
	m_vertex_buffer = m_backend->createBuffer( eBufferType::Buffer_Vertex );
	m_backend->bufferData( m_vertex_buffer, m_points.data(), m_points.size() * sizeof( sPoint ) );

	cVertexLayout layout;
	layout.push<double>( 3 );
	layout.push<double>( 4 );

	m_backend->bindVertexLayout( layout );

	m_model_location = m_backend->getUniformLocation( m_shader, "model" );
	m_view_location = m_backend->getUniformLocation( m_shader, "view" );
	m_proj_location = m_backend->getUniformLocation( m_shader, "proj" );
	m_focal_length_location = m_backend->getUniformLocation( m_shader, "focal_length" );

	for ( int i = 0; i < m_points.size(); i++ )
	{
		m_octree->addPoint( &m_points[ i ] );
	}
	m_octree->recalculate();
}

void cSceneGame::destroy( void )
{
	delete m_octree;
	m_octree = nullptr;

	m_points.clear();
	m_render_points.clear();
}

#define KEY_RIGHT 262
#define KEY_LEFT 263
#define KEY_UP 265
#define KEY_DOWN 264

#define KEY_LEFT_SHIFT 340
#define KEY_LEFT_CONTROL 341
#define KEY_SPACE 32

void cSceneGame::onRawInput( sInputInfo* _info )
{
	if ( _info->buttondown )
	{
		switch ( _info->key )
		{
		case KEY_RIGHT:
			m_input_yaw += 1;
			break;
		case KEY_LEFT:
			m_input_yaw -= 1;
			break;
		case KEY_UP:
			m_input_pitch -= 1;
			break;
		case KEY_DOWN:
			m_input_pitch += 1;
			break;

		case KEY_SPACE:
			m_run ^= 1;
			break;

		case KEY_LEFT_CONTROL:
			m_input_zoom -= 1;
			break;
		case KEY_LEFT_SHIFT:
			m_input_zoom += 1;
			break;

		case 'W':
			m_input_z += 1;
			break;
		case 'S':
			m_input_z -= 1;
			break;
		case 'A':
			m_input_x += 1;
			break;
		case 'D':
			m_input_x -= 1;
			break;
		case 'E':
			m_input_y -= 1;
			break;
		case 'Q':
			m_input_y += 1;
			break;
		}

	}
	else if( _info->buttonup )
	{
		switch ( _info->key )
		{
		case KEY_RIGHT:
			m_input_yaw -= 1;
			break;
		case KEY_LEFT:
			m_input_yaw += 1;
			break;
		case KEY_UP:
			m_input_pitch += 1;
			break;
		case KEY_DOWN:
			m_input_pitch -= 1;
			break;

		case KEY_LEFT_CONTROL:
			m_input_zoom += 1;
			break;
		case KEY_LEFT_SHIFT:
			m_input_zoom -= 1;
			break;

		case 'W':
			m_input_z -= 1;
			break;
		case 'S':
			m_input_z += 1;
			break;
		case 'A':
			m_input_x -= 1;
			break;
		case 'D':
			m_input_x += 1;
			break;

		case 'E':
			m_input_y += 1;
			break;
		case 'Q':
			m_input_y -= 1;
			break;

		case 'F':
			m_use_octree ^= 1;
			break;
		}
	}

	if ( _info->buttondown || _info->repeat )
	{
		if ( _info->key == 'G' )
		{
			if ( !m_run )
				updateUniverse( 3.155e12 );
		}
	}
}

/* formula derived from https://beltoforion.de/en/barnes-hut-galaxy-simulator/ */
wv::cVector3<double> computeForce( wv::cVector3d& _1p, wv::cVector3d& _2p, double& _1m, double& _2m )
{
	double mm = _1m * _2m;
	wv::cVector3d rirj = _1p - _2p;
	double magnitude = rirj.length();
	return ( rirj / pow( magnitude, 3 ) ) * mm;
}

void cSceneGame::updateUniverse( double _delta_time )
{
	for ( int i = 0; i < m_points.size(); i++ )
	{
		wv::cVector3d f;

		for ( int j = 0; j < m_points.size(); j++ )
		{
			if ( i == j )
				continue;

			sPoint& m1 = m_points[ i ];
			sPoint& m2 = m_points[ j ];

			f -= computeForce( m1.position, m2.position, m1.mass, m2.mass );
		}

		m_points[ i ].velocity += ( f * BIG_G ) / m_points[ i ].mass * _delta_time;
	}

	for ( int i = 0; i < m_points.size(); i++ )
	{
		m_points[ i ].last_position = m_points[ i ].position;
		m_points[ i ].position += m_points[ i ].velocity * _delta_time;
		m_render_points[ i ].position = m_points[ i ].position;
	}

	if( m_use_octree )
		m_octree->recalculate();

}

void cSceneGame::update( double _delta_time )
{
	m_pitch += _delta_time * m_input_pitch;
	m_yaw += _delta_time * m_input_yaw;
	m_zoom += _delta_time * m_input_zoom * m_zoom_speed;

	float move_z = cos( m_yaw ) * m_input_z + sin( m_yaw ) * m_input_x;
	float move_x = -sin( m_yaw ) * m_input_z + cos( m_yaw ) * m_input_x;

	float zoom = pow( 1.1f, -m_zoom );
	float speed = zoom * 10.0f * _delta_time;

	m_pos_z += move_z * speed;
	m_pos_x += move_x * speed;
	m_pos_y += m_input_y * speed;

	if ( m_run )
		updateUniverse( 3.155e14 );

	/* buffer new data */
	m_backend->bufferData( m_vertex_buffer, m_render_points.data(), m_render_points.size() * sizeof( sRenderPoint ) );
}

void cSceneGame::draw( void )
{
	const float FOV = 45.0f;
	cWindow& window = *cApplication::getInstance().getWindow();
	const double scale = 3.5e-20; /* a galaxy is very big */
	float aspect = window.getAspect();

	float zoom = pow( 1.1f, -m_zoom );

	m_focal_length = ( window.getWidth() / 2.0f ) / tan( FOV / 2.0f );

	/* create transforms */
	glm::mat4 model = glm::mat4( 1.0 );
	model = glm::scale( model, glm::vec3{ scale, scale, scale } );

	glm::mat4 view = glm::mat4( 1.0 );
	view = glm::translate( view, glm::vec3{ 0.0, 0.0, 30.0f * -zoom } );
	view = glm::rotate( view, m_pitch, glm::vec3{ 1.0f, 0.0f, 0.0f } );
	view = glm::rotate( view, m_yaw, glm::vec3{ 0.0f, 1.0f, 0.0f } );
	view = glm::translate( view, glm::vec3{ m_pos_x, m_pos_y, m_pos_z } );
	
	glm::mat4 projection = glm::perspective( glm::radians( 45.0f ), aspect, 0.0000001f, 10000.0f );

	/* apply transform uniforms */
	m_backend->useShaderProgram( m_shader );
	m_backend->setUniformMat4f( m_model_location, glm::value_ptr( model ) );
	m_backend->setUniformMat4f( m_view_location, glm::value_ptr( view ) );
	m_backend->setUniformMat4f( m_proj_location, glm::value_ptr( projection ) );
	m_backend->setUniformFloat( m_focal_length_location, m_focal_length );

	/* draw call */
	m_backend->bindVertexArray( m_vertex_array );
	m_backend->drawArrays( (unsigned int)m_points.size(), eDrawMode::DrawMode_Points );
	m_backend->bindVertexArray( 0 );

	/* draw octree */
	if ( m_use_octree )
		m_octree->drawNodeTree( scale, view, projection );

}
