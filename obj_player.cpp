#include "obj_player.h"
#include "Game.h"
#include "Globals.h"
#include "obj_burntplayer.h"
#include "obj_fish.h"
#include "obj_sparkle.h"
#include "obj_toast.h"

obj_player::obj_player(Game *_game) : Entity(_game)
{
}
void obj_player::onDestroy()
{
	
}
void obj_player::init()
{
	hitbox = ww::Rectanglei(0,0,16,32);
	type = OBJECT_PLAYER;
	vspeed = 0;
	faceLeft = true;
	hurtTimer = 0;
	shootTick = 0;
}
void obj_player::update()
{
	if (game->hp <= 0)
	{
		printf("MAKE DA SPARKLE\n");
		((obj_sparkle*)game->addEntity(new obj_sparkle(game),x-8,y))->sparkleType = 5;
		destroy();
	}
	if (hurtTimer > 0)
		hurtTimer--;
	else
	{
		if (collideObject(x,y,OBJECT_CRAWLER))
		{
			hurtTimer = 30;
			game->hp--;
		}
	}
	if (hasAppleWeapon)
	{
		if (ww::input::keyboard::isKeyDown(ww::input::key::Left))
			x -= 8*(hasSpeedWeapon?2:1);
		if (ww::input::keyboard::isKeyDown(ww::input::key::Right))
			x += 8*(hasSpeedWeapon?2:1);
		if (ww::input::keyboard::isKeyDown(ww::input::key::Up))
			y -= 8*(hasSpeedWeapon?2:1);
		if (ww::input::keyboard::isKeyDown(ww::input::key::Down))
			y += 8*(hasSpeedWeapon?2:1);
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
		faceLeft = true;
		if (!collideSolidTile(x-4*(hasSpeedWeapon?2:1),y) && !collideObject(x-4*(hasSpeedWeapon?2:1),y,OBJECT_DELILAH))
			x -= 4*(hasSpeedWeapon?2:1);
	}
	if (ww::input::keyboard::isKeyDown(ww::input::key::Right))
	{
		faceLeft = false;
		if (!collideSolidTile(x+4*(hasSpeedWeapon?2:1),y) && !collideObject(x+4*(hasSpeedWeapon?2:1),y,OBJECT_DELILAH))
			x += 4*(hasSpeedWeapon?2:1);
	}
	if (ww::input::keyboard::isKeyDown(ww::input::key::Space))
	{
		if (hasToasterWeapon && shootTick % 2 == 0)
		{
			int bonus = 0;//4*(hasSpeedWeapon?2:1)*(ww::input::keyboard::isKeyDown(ww::input::key::Right) - ww::input::keyboard::isKeyDown(ww::input::key::Left));
			((obj_toast*)game->addEntity(new obj_toast(game),x,y+8))->hspeed = bonus + ((faceLeft)?(-10):(10));
		}
	}
	if (ww::input::keyboard::isKeyPressed(ww::input::key::Space))
	{
		if (hasFishWeapon && !hasToasterWeapon)
		{
			int bonus = 0;//4*(hasSpeedWeapon?2:1)*(ww::input::keyboard::isKeyDown(ww::input::key::Right) - ww::input::keyboard::isKeyDown(ww::input::key::Left));
			((obj_fish*)game->addEntity(new obj_fish(game),x,y+8))->hspeed = bonus + ((faceLeft)?(-6):(6));
		}
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

	c = collideObject(x,y,OBJECT_POWERUP_TOASTER);
	if (c)
	{
		hasToasterWeapon = true;
		c->destroy();
		powerupCountdown = 30;
		game->showMessage("STAY TOASTY MY FRIEND");
	}

	c = collideObject(x,y,OBJECT_POWERUP_APPLE);
	if (c)
	{
		hasAppleWeapon = true;
		c->destroy();
		powerupCountdown = 30;
		game->showMessage("THE END");
	}

	c = collideObject(x,y,OBJECT_POWERUP_SPEED);
	if (c)
	{
		hasSpeedWeapon = true;
		c->destroy();
		powerupCountdown = 30;
		game->showMessage("GOTTA GO FSAT");
		x = 8 * (x/8);
	}

	if (collideTile(x,y,TILE_LAVA))
	{
		game->addEntity(new obj_burntplayer(game),x,y);
		destroy();
	}
	shootTick++;
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
		if (hurtTimer > 0 && (hurtTimer/4 % 2))
		{
			sprPlayerWhite->setColor(0xFFFFFFFF);
			sprPlayerWhite->setPosition(x,y);
			batch->pushsprite(sprPlayerWhite);
		}
		else
		{
			sprTiles[OBJECT_PLAYER]->setPosition(x,y);
			batch->pushsprite(sprTiles[OBJECT_PLAYER]);
		}
	}
}