#include "obj_burntplayer.h"
#include "Game.h"
#include "Globals.h"

obj_burntplayer::obj_burntplayer(Game *_game) : Entity(_game)
{
}
void obj_burntplayer::init()
{
	vspeed = -10;
}
void obj_burntplayer::update()
{
	vspeed += 1;
	y += vspeed;
	if (y > game->viewRect.y + game->viewRect.height)
	{
		game->clearAllEntities();
		game->reconsitutePlayer();
	}
}
void obj_burntplayer::render(ww::gfx::VertexBatch *batch)
{
	sprPlayerBurnt->setPosition(x,y);
	batch->pushsprite(sprPlayerBurnt);
}