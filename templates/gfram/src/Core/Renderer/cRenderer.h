#pragma once

class iBackend;
class iShader;
class cWindow;
class cPointCloud;

class cRenderer 
{
public:
	enum eBackendType
	{
		OpenGL,
		Direct3D11
	};

	 cRenderer( void );
	~cRenderer( void );
	
	void create( cWindow& _window, cRenderer::eBackendType _backend );

	void clear( unsigned int _color );

	void beginFrame( void );
	void endFrame  ( void );

	unsigned int getDefaultShader( void ) const { return m_shader_default; }
	
	void drawPointCloud( cPointCloud& _point_cloud );

private:
	void createDefaultShader( void );

	iBackend* m_backend = nullptr;

	const char* m_default_vertex_shader_source = 
		"#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";

	const char* m_default_fragment_shader_source =
		"#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\0";
	
	unsigned int m_shader_default = 0;
};
