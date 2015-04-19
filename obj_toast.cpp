#include "obj_toast.h"
#include "Game.h"
#include "Globals.h"

obj_toast::obj_toast(Game *_game) : Entity(_game)
{
}
void obj_toast::init()
{
	hitbox = ww::Rectanglei(2,3,12,11);
	type = OBJECT_TOAST;
	hspeed = 0;
	xstart = x;
}
void obj_toast::update()
{
	x += hspeed;
	y += 4*sin(abs(x-xstart) * M_PI / 20.f);
	if (collideSolidTile(x,y))
		destroy();
}
void obj_toast::render(ww::gfx::VertexBatch *batch)
{
	sprToast->setPosition(x,y);
	batch->pushsprite(sprToast);
}