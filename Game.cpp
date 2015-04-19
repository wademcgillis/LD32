#include "Game.h"
#include "Globals.h"
#include "obj_player.h"
#include "obj_crawler.h"
#include "obj_burntplayer.h"

class obj_fishpowerup : public Entity
{
public:
	int frame;
public:
	obj_fishpowerup(Game *_game) : Entity(_game)
	{
	}
	void init()
	{
		hitbox = ww::Rectanglei(0,0,32,32);
		type = OBJECT_POWERUP_FISH;
		frame = 0;
		if (hasFishWeapon)
			destroy();
	}
	void render(ww::gfx::VertexBatch *batch)
	{
		frame++;
		if (frame == 32)
			frame = 0;
		sprPowerupOrb[((frame/4) % 4)]->setPosition(x,y + (int)(2*sin(frame * M_PI/16.f)));
		batch->pushsprite(sprPowerupOrb[((frame/4) % 4)]);
		sprFishWeapon[0]->setPosition(x+8,y+8 + (int)(4*sin(frame * M_PI/16.f)));
		batch->pushsprite(sprFishWeapon[0]);
	}
};

class obj_toasterpowerup : public Entity
{
public:
	int frame;
public:
	obj_toasterpowerup(Game *_game) : Entity(_game)
	{
	}
	void init()
	{
		hitbox = ww::Rectanglei(0,0,32,32);
		type = OBJECT_POWERUP_TOASTER;
		frame = 0;
		if (hasToasterWeapon)
			destroy();
	}
	void render(ww::gfx::VertexBatch *batch)
	{
		frame++;
		if (frame == 32)
			frame = 0;
		sprPowerupOrb[((frame/4) % 4)]->setPosition(x,y + (int)(2*sin(frame * M_PI/16.f)));
		batch->pushsprite(sprPowerupOrb[((frame/4) % 4)]);
		sprToasterWeapon->setPosition(x+8,y+8 + (int)(4*sin(frame * M_PI/16.f)));
		batch->pushsprite(sprToasterWeapon);
	}
};

class obj_applepowerup : public Entity
{
public:
	int frame;
public:
	obj_applepowerup(Game *_game) : Entity(_game)
	{
	}
	void init()
	{
		hitbox = ww::Rectanglei(0,0,32,32);
		type = OBJECT_POWERUP_APPLE;
		frame = 0;
		if (hasAppleWeapon)
			destroy();
	}
	void render(ww::gfx::VertexBatch *batch)
	{
		frame++;
		if (frame == 32)
			frame = 0;
		sprPowerupOrb[((frame/4) % 4)]->setPosition(x,y + (int)(2*sin(frame * M_PI/16.f)));
		batch->pushsprite(sprPowerupOrb[((frame/4) % 4)]);
		sprAppleWeapon->setPosition(x+8,y+8 + (int)(4*sin(frame * M_PI/16.f)));
		batch->pushsprite(sprAppleWeapon);
	}
};

void Game::showMessage(std::string message)
{
	messages.push_back(message);
}
Entity *Game::addEntity(Entity *ent, int x, int y)
{
	entities.push_back(ent);
	ent->init();
	ent->x = x;
	ent->y = y;
	return ent;
}
void Game::start()
{
	player = NULL;
	hasFishWeapon = false;
	viewRect = ww::Rectanglei(0,0,256,224);
	for(int i=0;i<leveldata->objects.size();i++)
	{
		if (leveldata->objects.at(i)->id == OBJECT_PLAYER)
			player = (obj_player*)addEntity(new obj_player(this),16 * leveldata->objects.at(i)->x,16 * leveldata->objects.at(i)->y);
	}
}
void Game::clearAllEntities()
{
	for(int i=0;i<entities.size();i++)
	{
		if (entities.at(i) != player)
			entities.at(i)->destroy();
	}
	//if (player)
	//	entities.push_back(player);
}
void Game::reconsitutePlayer()
{
	if (player == NULL)
		player = (obj_player*)addEntity(new obj_player(this),lastPlayerEntrance.x,lastPlayerEntrance.y);
}
void Game::run()
{
	ww::gfx::setRenderSubrect(ww::gfx::MakeRenderSubrect(0,0,ww::gfx::window_width,ww::gfx::window_height));
	glClearColor(0x0C/255.f,0xBE/255.f,0xFF/255.f,1.f);
	glClear(GL_COLOR_BUFFER_BIT);
	tex->bind();

	if (messages.size() == 0 && powerupCountdown == 0)
	{
		for(int i=0;i<entities.size();i++)
		{
			if (entities.at(i)->isDestroyed())
			{
				if (entities.at(i) == player)
					player = NULL;
				delete entities.at(i);
				entities.erase(entities.begin()+i);
				i--;
			}
			entities.at(i)->update();
			if (entities.at(i)->isDestroyed())
			{
				if (entities.at(i) == player)
					player = NULL;
				delete entities.at(i);
				entities.erase(entities.begin()+i);
				i--;
			}
		}
		for(int i=0;i<entities.size();i++)
		{
			if (entities.at(i)->isDestroyed())
			{
				delete entities.at(i);
				entities.erase(entities.begin()+i);
				i--;
			}
		}
	}

	if (powerupCountdown > 0)
		powerupCountdown--;

	bool inAView = false;
	if (player != NULL)
	{
		ww::Rectanglei testRect(player->x+8,player->y+16,1,1);
		for(int i=0;i<leveldata->views.size();i++)
		{
			inViewRect.x = leveldata->views.at(i).x * 16;
			inViewRect.y = leveldata->views.at(i).y * 16;
			inViewRect.width = leveldata->views.at(i).width * 16;
			inViewRect.height = leveldata->views.at(i).height * 16;
			if (testRect.intersects(inViewRect))
			{
				inAView = true;
				i = leveldata->views.size();
			}
		}
		viewRect.x = player->x+8-256/2;
		viewRect.y = player->y+16-224/2;
		if (inAView)
		{
			if (viewRect.x < inViewRect.x)
				viewRect.x = inViewRect.x;
			if (viewRect.y < inViewRect.y)
				viewRect.y = inViewRect.y;
			if (viewRect.x+viewRect.width > inViewRect.x+inViewRect.width)
				viewRect.x = inViewRect.x+inViewRect.width-viewRect.width;
			if (viewRect.y+viewRect.height > inViewRect.y+inViewRect.height)
				viewRect.y = inViewRect.y+inViewRect.height-viewRect.height;
		}
		if (!inViewRect.intersects(inViewRectPrevious)) // if the previous view doesn't intersect this one...
		{
			clearAllEntities();
			lastPlayerEntrance.x = player->x;
			lastPlayerEntrance.y = player->y;

			for(int i=0;i<leveldata->objects.size();i++)
			{
				ww::Rectanglei pt(16 * leveldata->objects.at(i)->x,16 * leveldata->objects.at(i)->y,1,1);
				if (pt.intersects(inViewRect))
				{
					if (leveldata->objects.at(i)->id == OBJECT_POWERUP_FISH)
						addEntity(new obj_fishpowerup(this),pt.x,pt.y);
					if (leveldata->objects.at(i)->id == OBJECT_CRAWLER)	
						addEntity(new obj_crawler(this),pt.x,pt.y);
				}
			}
			// we changed views! make new bad guys / whatever and stuff
		}
	}
	

	ww::gfx::setMatrix(NULL,-1,glm::value_ptr(glm::ortho((float)viewRect.x,(float)(viewRect.x+viewRect.width),(float)(viewRect.y+viewRect.height),(float)viewRect.y,-1.f,1.f)));
	vb->clear();

	drawTiles(vb, false, 0xFFFFFFFF, false, ww::Rectanglei(-2+viewRect.x/16,-2+viewRect.y/16,viewRect.width/16+4,viewRect.height/16+4));
	drawTiles(vb, true, 0xFFFFFFFF, false, ww::Rectanglei(-2+viewRect.x/16,-2+viewRect.y/16,viewRect.width/16+4,viewRect.height/16+4));

	//sprTiles[leveldata->objects.at(0)->id]->setPosition(px,py);
	//vb->pushsprite(sprTiles[leveldata->objects.at(0)->id]);

	for(int i=0;i<entities.size();i++)
		entities.at(i)->render(vb);

	if (powerupCountdown == 0)
	{
		if (messages.size() > 0)
		{
			int len = messages.at(0).length();
			int w = len/2;
			if (len % 2)
				w = 1 + len/2;

			int xx = viewRect.x + viewRect.width/2 - (8*w+16);
			int yy = viewRect.y + viewRect.height/2 - 24;

			// left
			sprMessagebox[0]->setPosition(xx,yy);
			vb->pushsprite(sprMessagebox[0]);
			sprMessagebox[3]->setPosition(xx,yy+16);
			vb->pushsprite(sprMessagebox[3]);
			sprMessagebox[6]->setPosition(xx,yy+32);
			vb->pushsprite(sprMessagebox[6]);

			for(int i=0;i<w;i++)
			{
				sprMessagebox[1]->setPosition(xx+16*(i+1),yy);
				vb->pushsprite(sprMessagebox[1]);
				sprMessagebox[4]->setPosition(xx+16*(i+1),yy+16);
				vb->pushsprite(sprMessagebox[4]);
				sprMessagebox[7]->setPosition(xx+16*(i+1),yy+32);
				vb->pushsprite(sprMessagebox[7]);
			}

			// right
			sprMessagebox[2]->setPosition(xx + 16*(w+1),yy);
			vb->pushsprite(sprMessagebox[2]);
			sprMessagebox[5]->setPosition(xx + 16*(w+1),yy+16);
			vb->pushsprite(sprMessagebox[5]);
			sprMessagebox[8]->setPosition(xx + 16*(w+1),yy+32);
			vb->pushsprite(sprMessagebox[8]);

			draw_text(vb,xx+16,yy+20,messages.at(0),1,0xFFFFFFFF);
			if (ww::input::keyboard::isKeyPressed(ww::input::key::Space))
				messages.erase(messages.begin());
		}
	}

	vb->update();
	vb->draw();

	inViewRectPrevious = inViewRect;
	if (ww::input::keyboard::isKeyPressed(ww::input::key::Escape))
	{
		end();
		IN_EDITOR = true;
	}
}
void Game::end()
{
	while(entities.size())
	{
		delete entities.at(0);
		entities.erase(entities.begin());
	}
}