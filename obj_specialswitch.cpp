#include "obj_specialswitch.h"
#include "Game.h"
#include "Globals.h"

obj_specialswitch::obj_specialswitch(Game *_game) : Entity(_game)
{
}
void obj_specialswitch::init()
{
	hitbox = ww::Rectanglei(0,0,22,20,5,7);
}
void obj_specialswitch::update()
{
	Entity *e1 = collideObject(x,y,OBJECT_FISH);
	Entity *e2 = collideObject(x,y,OBJECT_TOAST);
	if (e1 || e2)
	{
		if (e1)
			e1->destroy();
		if (e2)
			e2->destroy();
		game->swapSpecialBlocks();
	}
}
void obj_specialswitch::render(ww::gfx::VertexBatch *batch)
{
	sprSpecialSwitch[game->specialBlock]->setPosition(x,y);
	batch->pushsprite(sprSpecialSwitch[game->specialBlock]);
}