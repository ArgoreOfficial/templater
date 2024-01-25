#pragma once

#include <Core/Renderer/Backends/iBackend.h>
#include <Core/Renderer/cRenderer.h>
#include <Core/Math/Vector3.h>
#include <Core/Renderer/Framework/Shader.h>
#include <Core/Renderer/Framework/Buffer.h>
#include <Core/Renderer/Framework/VertexArray.h>

struct sNode
{
	sNode* children[ 8 ] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
	unsigned short child_pointer;

	wv::cVector3<double> com;
	double mass;
};

class cOctree
{
public:
	 cOctree();
	~cOctree();

	void create();
	void drawNodeTree();

private:
	
	cRenderer* m_renderer;
	iBackend* m_backend;

	sNode* m_root_node;

	hShaderProgram m_box_shader;

	hVertexArray m_debug_box_vao;
	sBuffer m_debug_box_vbo;

	int m_model_location = -1;
	int m_view_location = -1;
	int m_proj_location = -1;

};