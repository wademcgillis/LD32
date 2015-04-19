#ifndef __WADEMCGILLISLD32__obj_specialswitch_h__
#define __WADEMCGILLISLD32__obj_specialswitch_h__
#include "Entity.h"
class obj_specialswitch : public Entity
{
public:
public:
	obj_specialswitch(Game *game);
	void init();
	void update();
	void render(ww::gfx::VertexBatch *batch);
};
#endif