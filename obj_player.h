#ifndef __WADEMCGILLISLD32__obj_player_h__
#define __WADEMCGILLISLD32__obj_player_h__
#include "Entity.h"
class obj_player : public Entity
{
public:
	int vspeed;
	bool faceLeft;
	int hurtTimer;
	int shootTick;
public:
	obj_player(Game *game);
	void onDestroy();
	void init();
	void update();
	void render(ww::gfx::VertexBatch *batch);
};
#endif