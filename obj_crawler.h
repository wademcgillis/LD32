#ifndef __WADEMCGILLISLD32__obj_crawler_h__
#define __WADEMCGILLISLD32__obj_crawler_h__
#include "Entity.h"
class obj_crawler : public Entity
{
public:
	int hspeed;
	int vspeed;
	int frame;
	int hp;
	unsigned char hurtTimer;
public:
	obj_crawler(Game *game);
	void onDestroy();
	void init();
	void update();
	void render(ww::gfx::VertexBatch *batch);
};
#endif