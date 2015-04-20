#ifndef __WADEMCGILLISLD32__obj_speedpowerup_h__
#define __WADEMCGILLISLD32__obj_speedpowerup_h__
#include "Entity.h"
class obj_speedpowerup : public Entity
{
public:
	int frame;
public:
	obj_speedpowerup(Game *_game);
	void init();
	void render(ww::gfx::VertexBatch *batch);
};
#endif