#ifndef __WADEMCGILLISLD32__obj_burntplayer_h__
#define __WADEMCGILLISLD32__obj_burntplayer_h__
#include "Entity.h"
class obj_burntplayer : public Entity
{
public:
	int vspeed;
public:
	obj_burntplayer(Game *game);
	void init();
	void update();
	void render(ww::gfx::VertexBatch *batch);
};
#endif