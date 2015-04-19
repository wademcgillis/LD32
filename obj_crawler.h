#ifndef __WADEMCGILLISLD32__obj_crawler_h__
#define __WADEMCGILLISLD32__obj_crawler_h__
#include "Entity.h"
class obj_crawler : public Entity
{
public:
	int hspeed;
	int vspeed;
	int frame;
public:
	obj_crawler(Game *game);
	~obj_crawler();
	void init();
	void update();
	void render(ww::gfx::VertexBatch *batch);
};
#endif