#pragma once

class cRenderer;
class cWindow;
class iScene;

#include <Core/Misc/iSingleton.h>

class cApplication : public iSingleton<cApplication>
{
public:
	enum class eBackend
	{
		kGL3,
		kD3D11
	};

	 cApplication( void );
	~cApplication( void );

	void run    ( void );

private:

	cRenderer* m_renderer;

	cWindow* m_window;
	
	iScene* m_scene;

};
