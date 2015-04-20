#include "obj_speedpowerup.h"
#include "Game.h"
#include "Globals.h"

obj_speedpowerup::obj_speedpowerup(Game *_game) : Entity(_game)
{
}
void obj_speedpowerup::init()
{
	hitbox = ww::Rectanglei(0,0,32,32);
	type = OBJECT_POWERUP_SPEED;
	frame = 0;
	if (hasSpeedWeapon)
		destroy();
}
void obj_speedpowerup::render(ww::gfx::VertexBatch *batch)
{
	frame++;
	if (frame == 32)
		frame = 0;
	sprPowerupOrb[((frame/4) % 4)]->setPosition(x,y + (int)(2*sin(frame * M_PI/16.f)));
	batch->pushsprite(sprPowerupOrb[((frame/4) % 4)]);
	sprSpeedWeapon->setPosition(x+8,y+8 + (int)(4*sin(frame * M_PI/16.f)));
	batch->pushsprite(sprSpeedWeapon);
}