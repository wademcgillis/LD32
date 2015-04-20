#include "obj_sparkle.h"
#include "Game.h"
#include "Globals.h"
#include "Sounds.h"

obj_sparkle::obj_sparkle(Game *_game) : Entity(_game)
{
}
void obj_sparkle::init()
{
	hitbox = ww::Rectanglei(0,0,32,32);
	type = -1;
	sparkleType = 0;
	frame = 17;
	sfx_sparkle.play();
}
void obj_sparkle::update()
{
	frame--;
	if (frame == 0)
	{
		printf("WOOWOO %i\n",sparkleType);
		if (sparkleType == 5)
		{
			printf("WAWA\n");
			game->clearAllEntities();
			game->reconsitutePlayer();
		}
		destroy();
	}
}
void obj_sparkle::render(ww::gfx::VertexBatch *batch)
{
	ww::gfx::Sprite *spr = NULL;
	if (sparkleType == 5)
		spr = sprPlayerSparkle[(frame/4)%2];
	if (sparkleType == 10)
		spr = sprCrawlerSparkle[(frame/4)%2];
	if (sparkleType == 15)
		spr = sprBlueSpinyRodentSparkle[(frame/4)%2];
	spr->setPosition(x,y);
	vb->pushsprite(spr);
}