#include "obj_bluespinyrodent.h"
#include "Game.h"
#include "Globals.h"
#include "obj_sparkle.h"
#include "obj_speedpowerup.h"

obj_bluespinyrodent::obj_bluespinyrodent(Game *_game) : Entity(_game)
{
}
void obj_bluespinyrodent::onDestroy()
{
	game->fightingBoss = false;
}
void obj_bluespinyrodent::init()
{
	hitbox = ww::Rectanglei(0,0,96,96);
	type = OBJECT_BLUESPINYRODENT;
	frame = 0;
	hurtTimer = 0;
	

	hspeed = 16 * ((rand()%2)?1:-1);
	difficultyScale = 5;
	myHP = difficultyScale;

	if (hasSpeedWeapon)
	{
		destroy();
	}
	else
	{
		game->fightingBoss = true;
		game->bossName = "SPINY";
		game->bossHP = 15;
		game->bossMaxHP = 15;
	}
}
void obj_bluespinyrodent::update()
{
	if (hurtTimer > 0)
		hurtTimer--;
	Entity *e = NULL;
	e = collideObject(x,y,OBJECT_FISH);
	if (e)
	{
		e->destroy();
		if (hurtTimer == 0)
		{
			myHP--;
			while(myHP <= 0)
			{
				game->bossHP--;
				myHP += difficultyScale;
			}
			hurtTimer = 30;
			if (game->bossHP <= 0)
			{
				((obj_sparkle*)game->addEntity(new obj_sparkle(game),x,y))->sparkleType = 15;
				((obj_sparkle*)game->addEntity(new obj_sparkle(game),x+32,y))->sparkleType = 15;
				((obj_sparkle*)game->addEntity(new obj_sparkle(game),x+64,y))->sparkleType = 15;
				((obj_sparkle*)game->addEntity(new obj_sparkle(game),x,y+32))->sparkleType = 15;
				((obj_sparkle*)game->addEntity(new obj_sparkle(game),x+32,y+32))->sparkleType = 15;
				((obj_sparkle*)game->addEntity(new obj_sparkle(game),x+64,y+32))->sparkleType = 15;
				((obj_sparkle*)game->addEntity(new obj_sparkle(game),x,y+64))->sparkleType = 15;
				((obj_sparkle*)game->addEntity(new obj_sparkle(game),x+32,y+64))->sparkleType = 15;
				((obj_sparkle*)game->addEntity(new obj_sparkle(game),x+64,y+64))->sparkleType = 15;
				game->addEntity(new obj_speedpowerup(game),x+32,y+32);
				destroy();
			}
		}
	}
	e = collideObject(x,y,OBJECT_TOAST);
	if (e)
	{
		e->destroy();
		if (hurtTimer == 0)
		{
			myHP--;
			myHP--;
			while(myHP <= 0)
			{
				game->bossHP--;
				myHP += difficultyScale;
			}
			hurtTimer = 30;
			if (game->bossHP <= 0)
			{
				((obj_sparkle*)game->addEntity(new obj_sparkle(game),x,y))->sparkleType = 15;
				((obj_sparkle*)game->addEntity(new obj_sparkle(game),x+32,y))->sparkleType = 15;
				((obj_sparkle*)game->addEntity(new obj_sparkle(game),x+64,y))->sparkleType = 15;
				((obj_sparkle*)game->addEntity(new obj_sparkle(game),x,y+32))->sparkleType = 15;
				((obj_sparkle*)game->addEntity(new obj_sparkle(game),x+32,y+32))->sparkleType = 15;
				((obj_sparkle*)game->addEntity(new obj_sparkle(game),x+64,y+32))->sparkleType = 15;
				((obj_sparkle*)game->addEntity(new obj_sparkle(game),x,y+64))->sparkleType = 15;
				((obj_sparkle*)game->addEntity(new obj_sparkle(game),x+32,y+64))->sparkleType = 15;
				((obj_sparkle*)game->addEntity(new obj_sparkle(game),x+64,y+64))->sparkleType = 15;
				game->addEntity(new obj_speedpowerup(game),x+32,y+32);
				destroy();
			}
		}
	}

	x += hspeed;
	if (collideSolidTile(x,y))
	{
		while(collideSolidTile(x,y))
		{
			if (hspeed < 0)
				x += 1;
			if (hspeed > 0)
				x -= 1;
		}
		hspeed = -hspeed;
	}

	frame++;
	frame++;
	if (frame == 16)
		frame = 0;
	//frame++;
	//if (frame == 8)
	//	frame = 0;
}
void obj_bluespinyrodent::render(ww::gfx::VertexBatch *batch)
{
	int F = frame/8+2*(hurtTimer > 0);
	sprBlueSpinyRodent[F]->setFlipX(hspeed > 0);
	sprBlueSpinyRodent[F]->setPosition(x,y);
	batch->pushsprite(sprBlueSpinyRodent[F]);
}