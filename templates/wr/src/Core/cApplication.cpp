#include "cApplication.h"

#include <App/cSceneGame.h>

#include <Core/cWindow.h>
#include <Core/Renderer/cRenderer.h>

cApplication::cApplication() :
	m_window{ new cWindow() },
	m_renderer{ new cRenderer() },
	m_scene{ new cSceneGame() }
{
}

cApplication::~cApplication()
{
	m_window->destroy();
}

void cApplication::onCreate()
{
	m_window->create( 800, 600, "renderer idfk" );
	m_renderer->create( *m_window, cRenderer::eBackendType::OpenGL );

	m_scene->create();
}

void cApplication::run()
{
	
	while ( !m_window->shouldClose() )
	{
		m_window->beginFrame();
		m_renderer->beginFrame();

		m_scene->update( 0.0f );

		m_renderer->clear( 0x000000FF );

		m_scene->draw();

		m_renderer->endFrame();
		m_window->endFrame();
	}
}

