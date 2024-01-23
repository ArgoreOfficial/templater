#pragma once

#include <Core/Scene/iScene.h>
#include <Core/Misc/cPointCloud.h>

class cSceneGame : public iScene
{
public:
	 cSceneGame( void );
	~cSceneGame( void );

	void create( void ) override;
	void update( double _delta_time ) override;
	void draw( void ) override;

private:

	cPointCloud m_point_cloud;

};
