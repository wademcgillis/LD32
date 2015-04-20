#include "Game.h"
#include "Globals.h"
#include "obj_player.h"
#include "obj_crawler.h"
#include "obj_burntplayer.h"
#include "obj_delilah.h"
#include "obj_specialswitch.h"
#include "obj_bluespinyrodent.h"
#include "obj_speedpowerup.h"
#include "Sounds.h"

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
	ent->x = x;
	ent->y = y;
	ent->init();
	return ent;
}
void Game::start()
{
	player = NULL;
	maxhp = 9;
	hp = 9;
	hasFishWeapon = false;
	hasToasterWeapon = false;
	hasAppleWeapon = false;
	hasSpeedWeapon = false;
	viewRect = ww::Rectanglei(0,0,256,192);
	for(int i=0;i<leveldata->objects.size();i++)
	{
		if (leveldata->objects.at(i)->id == OBJECT_PLAYER)
			player = (obj_player*)addEntity(new obj_player(this),16 * leveldata->objects.at(i)->x,16 * leveldata->objects.at(i)->y);
	}
	unsigned int x, y;
	for(unsigned int i=0;i<leveldata->width*leveldata->height;i++)
	{
		x = i % leveldata->width;
		y = i / leveldata->width;
		if (leveldata->getForegroundTile(x,y)->id == TILE_EMPTYSPECIALBLOCK || leveldata->getForegroundTile(x,y)->id == TILE_FULLSPECIALBLOCK)
			specialBlocks.push_back(ww::vec2dui(x,y));
	}
	currentMusicName = "```````````````````````````````````````````````````````````````````";
	currentMusic = &bgm_titletheme;
	currentMusic->loop();
	fightingBoss = false;
	bossName = "";
	bossHP = 0;
	bossMaxHP = 0;
	specialBlock = false;

	titleScreen = true;
	flashTimer = -5000;
}
void Game::swapSpecialBlocks()
{
	unsigned int x, y;
	for(int i=0;i<specialBlocks.size();i++)
	{
		x = specialBlocks.at(i).x;
		y = specialBlocks.at(i).y;

		if (leveldata->getForegroundTile(x,y)->id == TILE_EMPTYSPECIALBLOCK)
			leveldata->getForegroundTile(x,y)->id = TILE_FULLSPECIALBLOCK;
		else if (leveldata->getForegroundTile(x,y)->id == TILE_FULLSPECIALBLOCK)
			leveldata->getForegroundTile(x,y)->id = TILE_EMPTYSPECIALBLOCK;
	}
	specialBlock = !specialBlock;
}
void Game::clearAllEntities()
{
	for(int i=0;i<entities.size();i++)
	{
		if (entities.at(i) != player)
			entities.at(i)->destroy();
	}
	//entities.clear();
	//if (player)
	//	entities.push_back(player);
}
void Game::reconsitutePlayer()
{
	inViewRectPrevious = ww::Rectanglei(0,0,1,1);
	printf("RECONSITUTE PLAYER!!!!!!!!\n");
	hp = maxhp;
	if (player == NULL)
		player = (obj_player*)addEntity(new obj_player(this),lastPlayerEntrance.x,lastPlayerEntrance.y);
}
void Game::run()
{
	if (titleScreen)
	{
		static int nope = 10;
		if (nope > 0)
			nope--;
		ww::gfx::setRenderSubrect(ww::gfx::MakeRenderSubrect(0,0,ww::gfx::window_width,ww::gfx::window_height));
		glClearColor(0x0C/255.f,0xBE/255.f,0xFF/255.f,1.f);
		glClear(GL_COLOR_BUFFER_BIT);
		tex->bind();
		ww::gfx::setMatrix(NULL,-1,glm::value_ptr(glm::ortho(0.f,256.f,224.f,0.f,-1.f,1.f)));
		vb->clear();

		sprLogo->setPosition(16,8);
		vb->pushsprite(sprLogo);

		std::string str = "";
		
		if (flashTimer < 0 || (flashTimer > 0 && (flashTimer) % 2))
		{
			str = "-PRESS SPACE-";
			draw_text(vb,(256-str.length()*8)/2,144,str,1);
		}

		if (flashTimer > 0)
			flashTimer--;

		if (flashTimer == 0)
			titleScreen = false;

		if (ww::input::keyboard::isKeyPressed(ww::input::key::Space) && flashTimer < 0 && nope == 0)
		{
			flashTimer = 24;
			currentMusic->stop();
			currentMusic = &bgm_starttheme;
			currentMusic->loop();
		}

		str = "PRESS M TO MUTE";
		draw_text(vb,(256-str.length()*8)/2,192,str,1);

		str = "BY WADE MCGILLIS";
		draw_text(vb,(256-str.length()*8)/2,208,str,1);

		vb->update();
		vb->draw();

		return;
	}
	ww::gfx::setRenderSubrect(ww::gfx::MakeRenderSubrect(0,ww::gfx::window_height*(32.f/224.f),ww::gfx::window_width,ww::gfx::window_height*(1-32.f/224.f)));
	glClearColor(0x0C/255.f,0xBE/255.f,0xFF/255.f,1.f);
	glClear(GL_COLOR_BUFFER_BIT);
	tex->bind();

	if (messages.size() == 0 && powerupCountdown == 0)
	{
		for(int i=0;i<entities.size();i++)
		{
			/*if (entities.at(i)->isDestroyed())
			{
				if (entities.at(i) == player)
					player = NULL;
				entities.at(i)->onDestroy();
				delete entities.at(i);
				entities.erase(entities.begin()+i);
				i--;
				continue;
			}*/
			if (entities.at(i)->isDestroyed())
			{
				if (entities.at(i) == player)
					player = NULL;
				entities.at(i)->onDestroy();
				delete entities.at(i);
				entities.erase(entities.begin()+i);
				i--;
			}
			else
				entities.at(i)->update();
			if (i < entities.size())
				if (entities.at(i)->isDestroyed())
				{
					if (entities.at(i) == player)
						player = NULL;
				}
		}
		for(int i=0;i<entities.size();i++)
		{
			if (entities.at(i)->isDestroyed())
			{
				entities.at(i)->onDestroy();
				delete entities.at(i);
				entities.erase(entities.begin()+i);
				i--;
			}
		}
	}

	if (powerupCountdown > 0)
		powerupCountdown--;

	bool inAView = false;
	View *view = NULL;
	if (player != NULL)
	{
		ww::Rectanglei testRect(player->x+8,player->y+16,1,1);
		for(int i=0;i<leveldata->views.size();i++)
		{
			inViewRect.x = leveldata->views.at(i).view.x * 16;
			inViewRect.y = leveldata->views.at(i).view.y * 16;
			inViewRect.width = leveldata->views.at(i).view.width * 16;
			inViewRect.height = leveldata->views.at(i).view.height * 16;
			if (testRect.intersects(inViewRect))
			{
				view = &leveldata->views.at(i);
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

			if (!hasAppleWeapon)
			{
				std::string music = "~~~~~~~~~~~~~~~~~~~";
				if (view->extra == NULL)
				{
					printf("View extra is NULL!\n");
					music = "~~~~~~!!!!!!~~~~~~******";
				}
				else
				{
					printf("View extra exists!\n");
					music = readExtraString(view->extra,"MUSIC");
					dumpExtra(view->extra);
				}

				if (music != currentMusicName)
				{
					currentMusicName = music;
					printf("About to play: %s\n",currentMusicName.c_str());
					if (currentMusic != NULL)
						currentMusic->stop();
					if (currentMusicName == "SPINY")
						currentMusic = &bgm_spinytheme;
					else if (currentMusicName == "JUNGLE")
						currentMusic = &bgm_jungletheme;
					else if (currentMusicName == "PURPLE")
						currentMusic = &bgm_castletheme;
					else if (currentMusicName == "BOSS")
						currentMusic = &bgm_bosstheme;
					else if (currentMusicName == "SECRET")
						currentMusic = &bgm_secret;
					else if (currentMusicName == "CANYON")
						currentMusic = &bgm_canyontheme;
					else if (currentMusicName == "GARDEN")
						currentMusic = &bgm_gardentheme;
					else if (currentMusicName == "LAVA")
						currentMusic = &bgm_lavatheme;
					else
						currentMusic = NULL;
					if (currentMusic != NULL)
					{
						printf("YEEHAW!\n");
						if (hasSpeedWeapon)
							currentMusic->setFreq(1.3f);
						currentMusic->loop();
					}
				}
			}

			for(int i=0;i<leveldata->objects.size();i++)
			{
				ww::Rectanglei pt(16 * leveldata->objects.at(i)->x,16 * leveldata->objects.at(i)->y,1,1);
				if (pt.intersects(inViewRect))
				{
					if (leveldata->objects.at(i)->id == OBJECT_POWERUP_FISH)
						addEntity(new obj_fishpowerup(this),pt.x,pt.y);
					if (leveldata->objects.at(i)->id == OBJECT_CRAWLER)	
						addEntity(new obj_crawler(this),pt.x,pt.y);
					if (leveldata->objects.at(i)->id == OBJECT_DELILAH)	
						addEntity(new obj_delilah(this),pt.x,pt.y);
					if (leveldata->objects.at(i)->id == OBJECT_POWERUP_TOASTER)	
						addEntity(new obj_toasterpowerup(this),pt.x,pt.y);
					if (leveldata->objects.at(i)->id == OBJECT_POWERUP_APPLE)	
						addEntity(new obj_applepowerup(this),pt.x,pt.y);
					if (leveldata->objects.at(i)->id == OBJECT_SPECIAL_SWITCH)	
						addEntity(new obj_specialswitch(this),pt.x,pt.y);
					if (leveldata->objects.at(i)->id == OBJECT_BLUESPINYRODENT)	
						addEntity(new obj_bluespinyrodent(this),pt.x,pt.y);
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
	{
		if (entities.at(i) != player)
			if (!entities.at(i)->isDestroyed())
				entities.at(i)->render(vb);
	}
	if (player)
		player->render(vb);

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
			{
				if (messages.at(0) == "THE END")
				{
					if (currentMusic != NULL)
						currentMusic->stop();
					currentMusic = &bgm_endtheme;
					currentMusic->loop();
				}
				messages.erase(messages.begin());
			}
		}
	}

	vb->update();
	vb->draw();

	ww::gfx::setRenderSubrect(ww::gfx::MakeRenderSubrect(0,0,ww::gfx::window_width,(ww::gfx::window_height*32)/224));
	ww::gfx::setMatrix(NULL,-1,glm::value_ptr(glm::ortho(0.f,256.f,32.f,0.f,-1.f,1.f)));
	glClearColor(0.f,0.f,0.f,1.f);
	glClear(GL_COLOR_BUFFER_BIT);
	vb->clear();
	
	drawHP(vb,16,8,"GERALDINE",hp,maxhp);

	if (fightingBoss)
		drawHP(vb,256-16-16-8*bossName.length(),8-8*(bossMaxHP > bossName.length()),bossName,bossHP,bossMaxHP);

	//draw_text(vb,0,0,"",1);

	vb->update();
	vb->draw();


	inViewRectPrevious = inViewRect;
	if (ww::input::keyboard::isKeyPressed(ww::input::key::Escape) && !RELEASE)
	{
		end();
		IN_EDITOR = true;
	}
}
void Game::drawHP(ww::gfx::VertexBatch *batch, int x, int y, std::string name, int _hp, int _maxhp)
{
	int len = name.length();
	draw_text(batch,x,y,"-"+name+"-",1);
	for(int i=0;i<_maxhp;i++)
	{
		sprHUDHeart[(i < _hp)]->setPosition(x+8*(1+(i%len)),y+8*(1+(i/len)));
		batch->pushsprite(sprHUDHeart[(i < _hp)]);
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