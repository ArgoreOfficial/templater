#pragma once

#include <Core/Scene/iScene.h>

#include <Core/Renderer/Backends/iBackend.h>
#include <Core/Renderer/cRenderer.h>
#include <vector>

struct sPoint
{
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
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

	std::vector<sPoint> m_points;
};
