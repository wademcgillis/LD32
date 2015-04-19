#ifndef __WADEMCGILLISLD32__obj_fish_h__
#define __WADEMCGILLISLD32__obj_fish_h__
#include "Entity.h"
class obj_fish : public Entity
{
public:
	int vspeed;
	int hspeed;
public:
	obj_fish(Game *game);
	void init();
	void update();
	void render(ww::gfx::VertexBatch *batch);
};
#endif