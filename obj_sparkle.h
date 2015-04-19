#ifndef __WADEMCGILLISLD32__obj_sparkle_h__
#define __WADEMCGILLISLD32__obj_sparkle_h__
#include "Entity.h"
class obj_sparkle : public Entity
{
public:
	int frame;
	int sparkleType;
public:
	obj_sparkle(Game *game);
	void init();
	void update();
	void render(ww::gfx::VertexBatch *batch);
};
#endif