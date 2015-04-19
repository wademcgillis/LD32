#ifndef __WADEMCGILLISLD32__obj_delilah_h__
#define __WADEMCGILLISLD32__obj_delilah_h__
#include "Entity.h"
class obj_delilah : public Entity
{
public:
	char frame;
	char frameB4;
	unsigned char hurtTimer;
public:
	obj_delilah(Game *game);
	void onDestroy();
	void init();
	void update();
	void render(ww::gfx::VertexBatch *batch);
};
#endif