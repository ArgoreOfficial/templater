#include "cSceneGame.h"

cSceneGame::cSceneGame()
{

}

cSceneGame::~cSceneGame()
{

}

void cSceneGame::create( void )
{
	m_point_cloud.create( 3 );
	sPoint* data = m_point_cloud.data();

}

void cSceneGame::update( double _delta_time )
{
	
}

void cSceneGame::draw( void )
{
	
	/*
	
	cRenderer::drawPointCloud( m_pointcloud );

	*/

	// glUseProgram( renderer.getDefaultShader() );
	// glBindVertexArray( vao );
	// glDrawArrays( GL_TRIANGLES, 0, 3 );

}
