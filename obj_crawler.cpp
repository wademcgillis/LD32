#include "obj_crawler.h"
#include "Game.h"
#include "Globals.h"

obj_crawler::obj_crawler(Game *_game) : Entity(_game)
{
}
obj_crawler::~obj_crawler()
{
}
void obj_crawler::init()
{
	hitbox = ww::Rectanglei(0,0,32,32);
	type = OBJECT_CRAWLER;
	vspeed = 0;
	hspeed = rand()%2?-3:3;
	frame = 0;
}
void obj_crawler::update()
{
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
	sprCrawler[frame/4]->setPosition(x,y);
	batch->pushsprite(sprCrawler[frame/4]);
}