#include "cPointCloud.h"

#include <glad/glad.h>

cPointCloud::cPointCloud()
{

}

cPointCloud::~cPointCloud()
{

}

void cPointCloud::create( size_t _size )
{
	m_points = new sPoint[ _size ];
	memset( m_points, 0, _size * sizeof( sPoint ) );

	m_size = _size;
	
	glGenVertexArrays( 1, &m_vao );
	glBindVertexArray( m_vao );

	glGenBuffers( 1, &m_vbo );
	glBindBuffer( GL_ARRAY_BUFFER, m_vbo );
	glBufferData( GL_ARRAY_BUFFER, m_size * sizeof( sPoint ), m_points, GL_STATIC_DRAW);

	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof( float ), (void*)0 );
	glEnableVertexAttribArray( 0 );

}

void cPointCloud::update()
{

}
