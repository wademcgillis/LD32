#ifndef __WADEMCGILLISLD32__obj_player_h__
#define __WADEMCGILLISLD32__obj_player_h__
#include "Entity.h"
class obj_player : public Entity
{
public:
	int vspeed;
public:
	obj_player(Game *game);
	~obj_player();
	void init();
	void update();
	void render(ww::gfx::VertexBatch *batch);
};
#endif