#pragma once

#include <Core/Scene/iScene.h>

#include <Core/Renderer/Backends/iBackend.h>
#include <Core/Renderer/cRenderer.h>
#include <vector>

#include <App/cOctree.h>

class cVector3f
{
public:
	cVector3f():
		x{ 0.0f }, y{ 0.0f }, z{ 0.0f }
	{ }

	cVector3f( double _x, double _y, double _z ) :
		x{ _x }, y{ _y }, z{ _z }
	{ }


	~cVector3f() { }

	cVector3f operator+( const cVector3f& _other ) { return { x + _other.x, y + _other.y, z + _other.z }; }
	cVector3f operator-( const cVector3f& _other ) { return { x - _other.x, y - _other.y, z - _other.z }; }

	cVector3f& operator+=( const cVector3f& _other ) 
	{ 
		this->x += _other.x; 
		this->y += _other.y; 
		this->z += _other.z; 
		return *this;
	}

	cVector3f& operator-=( const cVector3f& _other )
	{
		this->x -= _other.x;
		this->y -= _other.y;
		this->z -= _other.z;
		return *this;
	}

	cVector3f operator*( const double& _scalar ) { return { x * _scalar, y * _scalar, z * _scalar }; }
	cVector3f operator/( const double& _scalar ) { return { x / _scalar, y / _scalar, z / _scalar }; }

	double x = 0.0f;
	double y = 0.0f;
	double z = 0.0f;


};

struct sPoint
{
	cVector3f position;
	cVector3f velocity;
	double mass;
	cVector3f color;
};

class cSceneGame : public iScene
{
public:
	 cSceneGame( void ) { };
	~cSceneGame( void ) { };

	void create( void ) override;
	void destroy( void ) override;
	void update( double _delta_time ) override;
	void draw( void ) override;

private:

	cRenderer* m_renderer;
	iBackend* m_backend;

	int m_model_location;
	int m_view_location;
	int m_proj_location;

	hShaderProgram m_shader;

	hVertexArray m_vertex_array;
	sBuffer m_vertex_buffer;

	cOctree m_octree;

	std::vector<sPoint> m_points;
};
