#include "obj_player.h"
#include "Game.h"
#include "Globals.h"
#include "obj_burntplayer.h"

obj_player::obj_player(Game *_game) : Entity(_game)
{
}
obj_player::~obj_player()
{
}
void obj_player::init()
{
	hitbox = ww::Rectanglei(0,0,16,32);
	type = OBJECT_PLAYER;
	vspeed = 0;
}
void obj_player::update()
{
	if (hasAppleWeapon)
	{
		if (ww::input::keyboard::isKeyDown(ww::input::key::Left))
			x -= 8;
		if (ww::input::keyboard::isKeyDown(ww::input::key::Right))
			x += 8;
		if (ww::input::keyboard::isKeyDown(ww::input::key::Up))
			y -= 8;
		if (ww::input::keyboard::isKeyDown(ww::input::key::Down))
			y += 8;
		return;
	}
	if (collideSolidTile(x,y+1))
	{
		if (ww::input::keyboard::isKeyDown(ww::input::key::Up))
			vspeed = -10;
	}
	else
	{
		vspeed += 1;
	}
	if (ww::input::keyboard::isKeyDown(ww::input::key::Left))
	{
		if (!collideSolidTile(x-4,y))
			x -= 4;
	}
	if (ww::input::keyboard::isKeyDown(ww::input::key::Right))
	{
		if (!collideSolidTile(x+4,y))
			x += 4;
	}
	y += vspeed;
	if (collideSolidTile(x,y))
	{
		Tile *voicebox = collideTile(x,y,TILE_VOICEBOX);
		while(collideSolidTile(x,y))
		{
			if (vspeed < 0)
				y += 1;
			if (vspeed > 0)
				y -= 1;
		}
		vspeed = 0;
		if (voicebox)
		{
			printf("It's a voicebox!\n");
			if (voicebox->extra)
			{
				std::string str = readExtraString(voicebox->extra,"MESSAGE");
				printf("\tsaying: %s\n",str.c_str());
				game->showMessage(str);
			}
			else
				printf("but no extra data?\n");
		}
	}

	Entity *c = collideObject(x,y,OBJECT_POWERUP_FISH);
	if (c)
	{
		hasFishWeapon = true;
		c->destroy();
		powerupCountdown = 30;
		game->showMessage("WISH FOR FISH");
	}

	if (collideTile(x,y,TILE_LAVA))
	{
		game->addEntity(new obj_burntplayer(game),x,y);
		destroy();
	}
}
void obj_player::render(ww::gfx::VertexBatch *batch)
{
	if (powerupCountdown)
	{
		sprPlayerWhite->setColor(0xFF000000 | RGB(128+127*(rand()%2),128+127*(rand()%2),128+127*(rand()%2)));
		sprPlayerWhite->setPosition(x,y);
		batch->pushsprite(sprPlayerWhite);
	}
	else
	{
		sprTiles[OBJECT_PLAYER]->setPosition(x,y);
		batch->pushsprite(sprTiles[OBJECT_PLAYER]);
	}
}