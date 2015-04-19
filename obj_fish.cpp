#include "obj_fish.h"
#include "Game.h"
#include "Globals.h"

obj_fish::obj_fish(Game *_game) : Entity(_game)
{
}
void obj_fish::init()
{
	hitbox = ww::Rectanglei(0,0,16,16);
	type = OBJECT_FISH;
	vspeed = -4;
	hspeed = 0;
}
void obj_fish::update()
{
	x += hspeed;
	y += vspeed;
	vspeed += 1;
	if (collideSolidTile(x,y))
		destroy();
}
void obj_fish::render(ww::gfx::VertexBatch *batch)
{
	if (hspeed < 0)
	{
		sprFishWeapon[0]->setPosition(x,y);
		batch->pushsprite(sprFishWeapon[0]);
	}
	else
	{
		sprFishWeapon[1]->setPosition(x,y);
		batch->pushsprite(sprFishWeapon[1]);
	}
}