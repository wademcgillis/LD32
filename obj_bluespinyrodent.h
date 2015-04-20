#ifndef __WADEMCGILLISLD32__obj_bluespinyrodent_h__
#define __WADEMCGILLISLD32__obj_bluespinyrodent_h__
#include "Entity.h"
class obj_bluespinyrodent : public Entity
{
public:
	char frame;
	unsigned char hurtTimer;
	int difficultyScale;
	int myHP;
	int hspeed;
public:
	obj_bluespinyrodent(Game *game);
	void onDestroy();
	void init();
	void update();
	void render(ww::gfx::VertexBatch *batch);
};
#endif