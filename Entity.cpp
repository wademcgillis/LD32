#include "Entity.h"
#include "Game.h"
#include "LevelData.h"
#include "Globals.h"

Entity::Entity(Game *_game)
{
	game = _game;
	destroyed = false;
	hitbox = ww::Rectanglei(0,0,1,1);
}
Entity::~Entity()
{
}
void Entity::init()
{
}
void Entity::update()
{
}
void Entity::render(ww::gfx::VertexBatch *batch)
{
}
Entity *Entity::collideObject(int _x, int _y, unsigned short objectType)
{
	hitbox.x = _x;
	hitbox.y = _y;
	Entity *e = NULL;
	ww::Rectanglei other;
	for(unsigned int i=0;i<game->entities.size();i++)
	{
		e = game->entities.at(i);
		if (e != this)
		{
			if (e->type == objectType)
			{
				other = e->hitbox;
				other.x = e->x;
				other.y = e->y;
				if (hitbox.intersects(other))
					return e;
			}
		}
	}
	return NULL;
}
Tile *Entity::collideTile(int _x, int _y, unsigned short tileType)
{
	hitbox.x = _x;
	hitbox.y = _y;
	for(int i=hitbox.x/16;i<=(hitbox.x+hitbox.width-1)/16;i++)
		for(int j=hitbox.y/16;j<=(hitbox.y+hitbox.height-1)/16;j++)
		{
			if (leveldata->getForegroundTile(i,j)->id == tileType)
				return leveldata->getForegroundTile(i,j);
		}
	return false;
}
bool Entity::collideSolidTile(int _x, int _y)
{
	hitbox.x = _x;
	hitbox.y = _y;
	for(int i=hitbox.x/16;i<=(hitbox.x+hitbox.width-1)/16;i++)
		for(int j=hitbox.y/16;j<=(hitbox.y+hitbox.height-1)/16;j++)
		{
			switch(leveldata->getForegroundTile(i,j)->id)
			{
			case TILE_GRASS: case TILE_DIRT: case TILE_BRICK: case TILE_WOOD: case TILE_BRIDGE: case TILE_VOICEBOX:
			case TILE_REDBRICK: case TILE_PURPLEBRICK: case TILE_JUNGLEGROUND: case TILE_VINEORB:
				return true;
				break;
			}
		}
	return false;
}
bool Entity::isDestroyed()
{
	return destroyed;
}
void Entity::destroy()
{
	destroyed = true;
}