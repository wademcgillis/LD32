#ifndef __WADEMCGILLISLD32__Game_h__
#define __WADEMCGILLISLD32__Game_h__
#include "Entity.h"

class obj_player;

class Game
{
public:
	ww::Rectanglei viewRect;
	ww::Rectanglei inViewRect;
	ww::Rectanglei inViewRectPrevious;
	std::vector<Entity*> entities;
	obj_player *player;
	std::vector<std::string> messages;
	ww::vec2di lastPlayerEntrance;

	char hp;
	char maxhp;

	bool fightingBoss;
	std::string bossName;
	char bossHP;
	char bossMaxHP;
	std::vector<ww::vec2dui> specialBlocks;
	bool specialBlock;
public:
	void start();
	void run();
	void end();
	Entity *addEntity(Entity *ent, int x, int y);
	void showMessage(std::string message);

	void clearAllEntities();
	void reconsitutePlayer();

	void drawHP(ww::gfx::VertexBatch *batch, int x, int y, std::string name, int hp, int maxhp);

	void swapSpecialBlocks();
};
#endif