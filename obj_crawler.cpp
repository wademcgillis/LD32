#include "obj_crawler.h"
#include "Game.h"
#include "Globals.h"
#include "obj_sparkle.h"
#include "Sounds.h"
obj_crawler::obj_crawler(Game *_game) : Entity(_game)
{
}
void obj_crawler::onDestroy()
{
}
void obj_crawler::init()
{
	hitbox = ww::Rectanglei(0,0,32,32);
	type = OBJECT_CRAWLER;
	vspeed = 0;
	hspeed = rand()%2?-3:3;
	frame = 0;
	hp = 1;
	hurtTimer = 0;
}
void obj_crawler::update()
{
	if (!sfx_lumpy.isPlaying())
		LD32PlaySoundWarbled(&sfx_lumpy);
	if (hurtTimer > 0)
		hurtTimer--;
	Entity *e = NULL;
	e = collideObject(x,y,OBJECT_FISH);
	if (e)
	{
		e->destroy();
		if (hurtTimer == 0)
		{
			hp--;
			hurtTimer = 30;
			if (hp <= 0)
			{
				((obj_sparkle*)game->addEntity(new obj_sparkle(game),x,y))->sparkleType = 10;
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
			hp--;
			hp--;
			hurtTimer = 30;
			if (hp <= 0)
			{
				((obj_sparkle*)game->addEntity(new obj_sparkle(game),x,y))->sparkleType = 10;
				destroy();
			}
		}
	}

	if (collideSolidTile(x+hspeed,y) || !collideSolidTile(x+((hspeed<0)?-16:16),y+1))
		hspeed = -hspeed;
	x += hspeed;
	if (!collideSolidTile(x,y+1))
		vspeed += 1;
	y += vspeed;
	if (collideSolidTile(x,y))
	{
		while(collideSolidTile(x,y))
		{
			if (vspeed < 0)
				y += 1;
			if (vspeed > 0)
				y -= 1;
		}
		vspeed = 0;
	}
	frame++;
	if (frame == 8)
		frame = 0;
}
void obj_crawler::render(ww::gfx::VertexBatch *batch)
{
	int F = (hurtTimer/4 % 2)?0:2*(hurtTimer > 0);
	sprCrawler[frame/4 + F]->setFlipX(hspeed > 0);
	sprCrawler[frame/4 + F]->setPosition(x,y);
	batch->pushsprite(sprCrawler[frame/4 + F]);
}