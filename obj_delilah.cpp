#include "obj_delilah.h"
#include "Game.h"
#include "Globals.h"
#include "obj_crawler.h"
#include "obj_sparkle.h"
#include "Sounds.h"

unsigned char anim[] = {1,0,3,0,1,0,3,0,1,0,3,0,2,0};

obj_delilah::obj_delilah(Game *_game) : Entity(_game)
{
}
void obj_delilah::onDestroy()
{
	printf("DEALLOC!!!! \n");
	game->fightingBoss = false;
}
void obj_delilah::init()
{
	hitbox = ww::Rectanglei(0,0,64,64);
	type = OBJECT_DELILAH;
	frame = 0;
	hurtTimer = 0;

	if (hasToasterWeapon)
	{
		destroy();
	}
	else
	{
		game->fightingBoss = true;
		game->bossName = "delilah";
		game->bossHP = 7;
		game->bossMaxHP = 7;
	}
}
void obj_delilah::update()
{
	if (hurtTimer > 0)
		hurtTimer--;
	Entity *e = NULL;
	e = collideObject(x,y,OBJECT_FISH);
	if (e)
	{
		e->destroy();
		if (anim[(frame)/8] == 2 && hurtTimer == 0)
		{
			game->bossHP--;
			hurtTimer = 30;
			if (game->bossHP <= 0)
			{
				((obj_sparkle*)game->addEntity(new obj_sparkle(game),x,y))->sparkleType = 10;
				((obj_sparkle*)game->addEntity(new obj_sparkle(game),x+32,y))->sparkleType = 10;
				((obj_sparkle*)game->addEntity(new obj_sparkle(game),x,y+32))->sparkleType = 10;
				((obj_sparkle*)game->addEntity(new obj_sparkle(game),x+32,y+32))->sparkleType = 10;
				destroy();
			}
		}
	}
	e = collideObject(x,y,OBJECT_TOAST);
	if (e)
	{
		e->destroy();
		if (anim[(frame)/8] == 2 && hurtTimer == 0)
		{
			game->bossHP--;
			game->bossHP--;
			hurtTimer = 30;
			if (game->bossHP <= 0)
			{
				((obj_sparkle*)game->addEntity(new obj_sparkle(game),x,y))->sparkleType = 10;
				((obj_sparkle*)game->addEntity(new obj_sparkle(game),x+32,y))->sparkleType = 10;
				((obj_sparkle*)game->addEntity(new obj_sparkle(game),x,y+32))->sparkleType = 10;
				((obj_sparkle*)game->addEntity(new obj_sparkle(game),x+32,y+32))->sparkleType = 10;
				destroy();
			}
		}
	}

	frame++;
	if (frame/8 == 14)
		frame = 0;

	if (anim[frame/8] == 2 && anim[frameB4/8] != 2)
	{
		((obj_crawler*)game->addEntity(new obj_crawler(game),x+16,y+32))->hspeed = -3;
		LD32PlaySoundWarbled(&sfx_burp1);
	}
	if (anim[frame/8] == 3 && anim[frameB4/8] == 0)
	{
		((obj_crawler*)game->addEntity(new obj_crawler(game),x+16,y+32))->hspeed = 3;
		LD32PlaySoundWarbled(&sfx_burp2);
	}
	frameB4 = frame;
	//frame++;
	//if (frame == 8)
	//	frame = 0;
}
void obj_delilah::render(ww::gfx::VertexBatch *batch)
{
	int N = anim[(frame/8)];
	if (N == 3)
		N = 1;
	int F = (hurtTimer/4 % 2)?0:3*(hurtTimer > 0);
	sprDelilah[N+F]->setPosition(x,y);
	batch->pushsprite(sprDelilah[N+F]);
}