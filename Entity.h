#ifndef __WADEMCGILLISLD32__Entity_h__
#define __WADEMCGILLISLD32__Entity_h__
#include <WadeWork/types.h>
#include <WadeWork/VertexBatch.h>
#include "LevelData.h"
class Game;

class Entity
{
protected:
	bool destroyed;
public:
	Game *game;
	int x, y;
	ww::Rectanglei hitbox;
	unsigned short type;
public:
	Entity(Game *game);
	~Entity();
	virtual void onDestroy();
	virtual void init();
	virtual void update();
	virtual void render(ww::gfx::VertexBatch *batch);
	Entity *collideObject(int x, int y, unsigned short objectType);
	Tile *collideTile(int x, int y, unsigned short tileType);
	bool collideSolidTile(int x, int y);
	bool isDestroyed();
	void destroy();
};
#endif