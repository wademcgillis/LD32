#ifndef __WADEMCGILLISLD32__obj_toast_h__
#define __WADEMCGILLISLD32__obj_toast_h__
#include "Entity.h"
class obj_toast : public Entity
{
public:
	int hspeed;
	int xstart;
public:
	obj_toast(Game *game);
	void init();
	void update();
	void render(ww::gfx::VertexBatch *batch);
};
#endif