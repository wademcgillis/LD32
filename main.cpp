#include <WadeWork/wadework.h>
#define MAIN
#include "Globals.h"
#include "Game.h"
#include "Sounds.h"

bool doinit();
bool dorun();
bool doend();

void doEditor();

Game *game;

#include <SFML/Window.hpp>
namespace ww
{
	namespace gfx
	{
		extern sf::Window *window;
	}
}

int WinMain(HINSTANCE hInstnace, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	ww::gfx::setWindowSize(256*3,224*3);
	ww::sys::setInitCallback(doinit);
	ww::sys::setTimerCallback(dorun);
	ww::sys::setDeinitCallback(doend);
	ww::sys::setTimerResolution(30);
	ww::sys::setup(ww::sys::CONFIG_OPENGL1 | ww::sys::CONFIG_DISABLE_OPENGL_DEPTHBUFFER);//ww::sys::CONFIG_DISABLE_OPENAL | 
}

bool doinit()
{
	ww::gfx::window->setTitle("¡WHOA pizza!");
	RELEASE = false;

	LD32LoadSound(&bgm_spinytheme,"resources/sfx/spinytheme.ogg");
	LD32LoadSound(&bgm_titletheme,"resources/sfx/titletheme.ogg");
	LD32LoadSound(&bgm_starttheme,"resources/sfx/starttheme.ogg");
	LD32LoadSound(&bgm_endtheme,"resources/sfx/endtheme.ogg");
	LD32LoadSound(&bgm_jungletheme,"resources/sfx/jungle.ogg");
	LD32LoadSound(&bgm_castletheme,"resources/sfx/purplecastle.ogg");
	LD32LoadSound(&bgm_bosstheme,"resources/sfx/delilah.ogg");
	LD32LoadSound(&bgm_secret,"resources/sfx/secret.ogg");
	LD32LoadSound(&bgm_gardentheme,"resources/sfx/gardentheme.ogg");
	LD32LoadSound(&bgm_canyontheme,"resources/sfx/canyontheme.ogg");
	LD32LoadSound(&bgm_lavatheme,"resources/sfx/lavatheme.ogg");

	LD32LoadSound(&sfx_burp1,"resources/sfx/burp1.ogg");
	LD32LoadSound(&sfx_burp2,"resources/sfx/burp2.ogg");
	LD32LoadSound(&sfx_obtain,"resources/sfx/obtain.ogg");
	LD32LoadSound(&sfx_lumpy,"resources/sfx/lumpy.ogg");
	LD32LoadSound(&sfx_fish,"resources/sfx/fish.ogg");
	LD32LoadSound(&sfx_toast,"resources/sfx/toast.ogg");
	LD32LoadSound(&sfx_lose,"resources/sfx/lose.ogg");
	LD32LoadSound(&sfx_jump,"resources/sfx/jump.ogg");
	LD32LoadSound(&sfx_sparkle,"resources/sfx/sparkle.ogg");

	setSoundEnabled(true);

	tex = new ww::gfx::Texture();
	tex->load("resources/gfx/texture.png");

	for(int i=0;i<64;i++)
	{
		font[i] = new ww::gfx::Sprite();
		font[i]->setSize(8,8);
		font[i]->setPosition(0,0);
		font[i]->setSubimageUVRect(0,tex->getUVRectFromIntRect(ww::Rectanglei(8*(i%8),192+8*(i/8),8,8)));
		font[i]->setColor(0xFFFFFFFF);
	}

	rectangle = new ww::gfx::Sprite();
	rectangle->setSize(1,1);
	rectangle->setPosition(1,1);
	rectangle->setSubimageUVRect(0,tex->getUVRectFromIntRect(ww::Rectanglei(256-16,256-16,16,16)));
	rectangle->setColor(0xFFFFFFFF);

	highlightybob[0] = new ww::gfx::Sprite();
	highlightybob[0]->setSize(48,48);
	highlightybob[0]->setPosition(0,0);
	highlightybob[0]->setSubimageUVRect(0,tex->getUVRectFromIntRect(ww::Rectanglei(64,256-48,48,48)));
	highlightybob[0]->setColor(0xFFFFFFFF);

	highlightybob[1] = new ww::gfx::Sprite();
	highlightybob[1]->setSize(48,48);
	highlightybob[1]->setPosition(0,0);
	highlightybob[1]->setSubimageUVRect(0,tex->getUVRectFromIntRect(ww::Rectanglei(112,256-48,48,48)));
	highlightybob[1]->setColor(0xFFFFFFFF);

	vb = new ww::gfx::VertexBatch();

	sprTiles[TILE_NONE] = MakeTileSprite(tex,224,240);
	sprTiles[TILE_DIRT] = MakeTileSprite(tex,32,0);
	sprTiles[TILE_GRASS] = MakeTileSprite(tex,48,0);
	sprTiles[TILE_FORESTDIRT] = MakeTileSprite(tex,32,16);
	sprTiles[TILE_FORESTGRASS] = MakeTileSprite(tex,48,16);
	sprTiles[TILE_BRICK] = MakeTileSprite(tex,64,0);
	sprTiles[TILE_SMALLBRICK] = MakeTileSprite(tex,64,16);
	sprTiles[TILE_BRIDGE] = MakeTileSprite(tex,80,0);
	//sprTiles[TILE_BRIDGE] = MakeTileSprite(tex,80,16);
	sprTiles[TILE_CHAIN_NE] = MakeTileSprite(tex,96,0);
	sprTiles[TILE_CHAIN_NW] = MakeTileSprite(tex,208,80);
	sprTiles[TILE_LEAVES] = MakeTileSprite(tex,96,16);
	sprTiles[TILE_WOOD] = MakeTileSprite(tex,112,0);
	sprTiles[TILE_STUMP] = MakeTileSprite(tex,112,16);
	sprTiles[TILE_SHRINE_LEFT] = MakeTileSprite(tex,80,32);
	sprTiles[TILE_SHRINE_RIGHT] = MakeTileSprite(tex,96,32);
	sprTiles[TILE_VOICEBOX] = MakeTileSprite(tex,48,32);
	sprTiles[TILE_LAVA] = MakeTileSprite(tex,144,32);
	sprTiles[TILE_REDSMALLBRICK] = MakeTileSprite(tex,64,32);
	sprTiles[TILE_PURPLESMALLBRICK] = MakeTileSprite(tex,80,64);
	sprTiles[TILE_REDBRICK] = MakeTileSprite(tex,48,64);
	sprTiles[TILE_PURPLEBRICK] = MakeTileSprite(tex,64,64);
	sprTiles[TILE_JUNGLEGROUND] = MakeTileSprite(tex,112,32);
	sprTiles[TILE_JUNGLEBACKGROUND] = MakeTileSprite(tex,208,48);
	sprTiles[TILE_VINE] = MakeTileSprite(tex,128,32);
	sprTiles[TILE_VINEORB] = MakeTileSprite(tex,208,64);
	sprTiles[TILE_REDDARKGROUND] = MakeTileSprite(tex,128,16);
	sprTiles[TILE_PURPLESMALLBRICKHOLE] = MakeTileSprite(tex,48,80);
	sprTiles[TILE_SMALLBRICKHOLE] = MakeTileSprite(tex,64,80);
	sprTiles[TILE_EMPTYSPECIALBLOCK] = MakeTileSprite(tex,128,80);
	sprTiles[TILE_FULLSPECIALBLOCK] = MakeTileSprite(tex,128,96);

	sprTiles[OBJECT_NONE] = MakeTileSprite(tex,0,0,256,256);
	sprTiles[OBJECT_NONE]->setSize(64,64);
	sprTiles[OBJECT_PLAYER] = MakeTileSprite(tex,0,0,16,32);
		sprPlayerWhite = MakeTileSprite(tex,16,0,16,32);
		sprPlayerBurnt = MakeTileSprite(tex,16,64,16,32);
	sprTiles[OBJECT_POWERUP_FISH] = MakeTileSprite(tex,96,48,32,32);
	sprTiles[OBJECT_CRAWLER] = MakeTileSprite(tex,224,0,32,32);
	sprTiles[OBJECT_POWERUP_TOASTER] = MakeTileSprite(tex,128,48,32,32);
	sprTiles[OBJECT_POWERUP_APPLE] = MakeTileSprite(tex,96,80,32,32);
	sprTiles[OBJECT_DELILAH] = MakeTileSprite(tex,352,0,64,64);
	sprTiles[OBJECT_BLUESPINYRODENT] = MakeTileSprite(tex,416,0,96,96);
	sprTiles[OBJECT_SPECIAL_SWITCH] = MakeTileSprite(tex,144,80,32,32);

	sprPowerupOrb[0] = MakeTileSprite(tex,0,160,32,32);
	sprPowerupOrb[1] = MakeTileSprite(tex,32,160,32,32);
	sprPowerupOrb[2] = MakeTileSprite(tex,64,160,32,32);
	sprPowerupOrb[3] = MakeTileSprite(tex,32,160,32,32);

	sprFishWeapon[0] = MakeTileSprite(tex,32,48);
	sprFishWeapon[1] = MakeTileSprite(tex,48,48);

	sprSpecialSwitch[0] = MakeTileSprite(tex,144,80,32,32);
	sprSpecialSwitch[1] = MakeTileSprite(tex,176,80,32,32);

	sprToast = MakeTileSprite(tex,80,48);
	sprToasterWeapon = MakeTileSprite(tex,64,48);
	sprAppleWeapon = MakeTileSprite(tex,32,64);
	sprSpeedWeapon = MakeTileSprite(tex,80,80);


	sprMessagebox[0] = MakeTileSprite(tex,112,160);
	sprMessagebox[1] = MakeTileSprite(tex,128,160);
	sprMessagebox[2] = MakeTileSprite(tex,144,160);

	sprMessagebox[3] = MakeTileSprite(tex,112,176);
	sprMessagebox[4] = MakeTileSprite(tex,128,176);
	sprMessagebox[5] = MakeTileSprite(tex,144,176);

	sprMessagebox[6] = MakeTileSprite(tex,112,192);
	sprMessagebox[7] = MakeTileSprite(tex,128,192);
	sprMessagebox[8] = MakeTileSprite(tex,144,192);


	sprCrawler[0] = MakeTileSprite(tex,224,0,32,32);
	sprCrawler[1] = MakeTileSprite(tex,224,32,32,32);
	sprCrawler[2] = MakeTileSprite(tex,256,0,32,32);
	sprCrawler[3] = MakeTileSprite(tex,256,32,32,32);

	sprCrawlerSparkle[0] = MakeTileSprite(tex,224,64,32,32);
	sprCrawlerSparkle[1] = MakeTileSprite(tex,256,64,32,32);
	sprPlayerSparkle[0] = MakeTileSprite(tex,224,96,32,32);
	sprPlayerSparkle[1] = MakeTileSprite(tex,256,96,32,32);
	sprBlueSpinyRodentSparkle[0] = MakeTileSprite(tex,224,128,32,32);
	sprBlueSpinyRodentSparkle[1] = MakeTileSprite(tex,256,128,32,32);

	sprBlueSpinyRodent[0] = MakeTileSprite(tex,416,0,96,96);
	sprBlueSpinyRodent[1] = MakeTileSprite(tex,416,96,96,96);
	sprBlueSpinyRodent[2] = MakeTileSprite(tex,416,192,96,96);
	sprBlueSpinyRodent[3] = MakeTileSprite(tex,416,288,96,96);

	sprPlay = MakeTileSprite(tex,160,224,32,32);
	sprView = MakeTileSprite(tex,192,224,32,32);
	sprObjects = MakeTileSprite(tex,64,192);
	sprForeground = MakeTileSprite(tex,80,192);
	sprBackground = MakeTileSprite(tex,96,192);

	sprHUDHeart[0] = MakeTileSprite(tex,8,80,8,8);
	sprHUDHeart[1] = MakeTileSprite(tex,0,80,8,8);

	sprDelilah[0] = MakeTileSprite(tex,352,0,64,64);
	sprDelilah[1] = MakeTileSprite(tex,352,64,64,64);
	sprDelilah[2] = MakeTileSprite(tex,352,128,64,64);
	sprDelilah[3] = MakeTileSprite(tex,288,0,64,64);
	sprDelilah[4] = MakeTileSprite(tex,288,64,64,64);
	sprDelilah[5] = MakeTileSprite(tex,288,128,64,64);

	sprLogo = MakeTileSprite(tex,0,256,224,128);

	for(int i=0;i<8;i++)
		sprLava[i] = MakeTileSprite(tex,144+2*i,32,16,16);

	leveldata = new LevelData();
	leveldata->reset(1024,1024);

	EDITOR_LAYER = OBJECT_LAYER;

	viewX = 0;
	viewY = 0;
	viewStartX = 0;
	viewStartY = 0;
	dragging = false;
	dragStartX = 0;
	dragStartY = 0;
	rectangleX = 0;
	rectangleY = 0;
	makingRectangle = false;
	editingFilename = false;
	filename = "";


	IN_EDITOR = true;

	CURRENT_TILE = TILE_NONE;
	CURRENT_OBJECT = OBJECT_NONE;

	game = new Game();

	FILE *check = fopen("resources/RELEASE.txt","rb");
	if (check != NULL)
	{
		RELEASE = true;
		fclose(check);
	}

	if (RELEASE)
	{
		leveldata->load("game.data");
		IN_EDITOR = false;
		game->start();
	}

	return true;
}

bool dorun()
{
	if (IN_EDITOR)
		doEditor();
	else
	{
		if (ww::input::keyboard::isKeyPressed(ww::input::key::M))
			setSoundEnabled(!soundEnabled);
		game->run();
	}
	return true;
}

bool doend()
{
	return true;
}

void doEditor()
{
	ww::gfx::setRenderSubrect(ww::gfx::MakeRenderSubrect(0,0,ww::gfx::window_width,ww::gfx::window_height));
	glClearColor(1.f,1.f,1.f,1.f);
	glClear(GL_COLOR_BUFFER_BIT);
	tex->bind();

	/*

											TOP MENU BAR

	*/

	ww::gfx::setRenderSubrect(ww::gfx::MakeRenderSubrect(0,0,ww::gfx::window_width,64));
	ww::gfx::setMatrix(NULL,-1,glm::value_ptr(glm::ortho(0.f,(float)ww::gfx::window_width,64.f,0.f,-1.f,1.f)));
	vb->clear();
	// DRAW THE MENU BAR
	static int overlayHeight = 64;
	draw_rectangle(vb, ww::Rectanglei(0, 0, ww::gfx::window_width, 64), (overlayHeight > 64)?0xE0000000:0xA0000000);

	// BUTTONS ON THE LEFT
	if (draw_bubblebutton(vb,8,8,sprPlay,0xFF0000FF))
	{
		IN_EDITOR = false;
		game->start();
	}
	if (draw_bubblebutton(vb,80,8,sprView,(EDITOR_LAYER == VIEW_LAYER)?0xFF00FF00:0))
		EDITOR_LAYER = VIEW_LAYER;
	if (draw_bubblebutton(vb,136,8,sprObjects,(EDITOR_LAYER == OBJECT_LAYER)?0xFF00FF00:0))
		EDITOR_LAYER = OBJECT_LAYER;
	if (draw_bubblebutton(vb,192,8,sprForeground,(EDITOR_LAYER == FOREGROUND_LAYER)?0xFF00FF00:0))
		EDITOR_LAYER = FOREGROUND_LAYER;
	if (draw_bubblebutton(vb,248,8,sprBackground,(EDITOR_LAYER == BACKGROUND_LAYER)?0xFF00FF00:0))
		EDITOR_LAYER = BACKGROUND_LAYER;

	if (overlayHeight > 64)
	{
		if (draw_button(vb,ww::Rectanglei(312,8,128,48),"CLOSE"))
		{
			overlayHeight = 64;
			editingFilename = false;
			EXTRAMODIFIER = NULL;
		}
	}
	else
	{
		if (draw_button(vb,ww::Rectanglei(312,8,128,48),"MORE") || EXTRAMODIFIER)
		{
			overlayHeight = ww::gfx::window_height;
		}
	}
	// RECENTLY USED
	draw_text(vb,456,24,"RECENTLY\n  USED",1);
	if (EDITOR_LAYER == FOREGROUND_LAYER || EDITOR_LAYER == BACKGROUND_LAYER)
	{
		for(unsigned int i=0;i<4;i++)
		{
			if (i < recent_tiles.size())
			{
				if (draw_bubblebutton(vb,536+56*i,8,sprTiles[recent_tiles.at(i)],recent_tiles.at(i)==CURRENT_TILE?0xFFFFFFFF:0))
					CURRENT_TILE = recent_tiles.at(i);
			}
			else
				draw_bubblebutton(vb,536+56*i,8,NULL);
		}
	}
	else if (EDITOR_LAYER == OBJECT_LAYER)
	{
		for(unsigned int i=0;i<4;i++)
		{
			if (i < recent_objects.size())
			{
				if (draw_bubblebutton(vb,536+56*i,8,sprTiles[recent_objects.at(i)],recent_objects.at(i)==CURRENT_OBJECT?0xFFFFFFFF:0))
					CURRENT_OBJECT = recent_objects.at(i);
			}
			else
				draw_bubblebutton(vb,536+56*i,8,NULL);
		}
	}
	else
	{
		for(unsigned int i=0;i<4;i++)
			draw_bubblebutton(vb,536+56*i,8,NULL);
	}
	vb->update();
	vb->draw();



	/*

											BIG SCREEN

	*/

	static int TILEGRID_WIDTH = ww::gfx::window_width;
	static int TILEGRID_HEIGHT = ww::gfx::window_height-16-64;

	if (ww::input::mouse::wheel() > 0)
	{
		TILEGRID_WIDTH /= pow(1.1f,abs(ww::input::mouse::wheel()));
		TILEGRID_HEIGHT /= pow(1.1f,abs(ww::input::mouse::wheel()));
		if (TILEGRID_WIDTH < ww::gfx::window_width)
			TILEGRID_WIDTH = ww::gfx::window_width;
		if (TILEGRID_HEIGHT < ww::gfx::window_height-16-64)
			TILEGRID_HEIGHT = ww::gfx::window_height-16-64;

	}
	if (ww::input::mouse::wheel() < 0)
	{
		TILEGRID_WIDTH *= pow(1.1f,abs(ww::input::mouse::wheel()));
		TILEGRID_HEIGHT *= pow(1.1f,abs(ww::input::mouse::wheel()));
	}

	ww::gfx::setRenderSubrect(ww::gfx::MakeRenderSubrect(0,64,ww::gfx::window_width,ww::gfx::window_height-64-16));
	ww::gfx::setMatrix(NULL,-1,glm::value_ptr(glm::ortho(0.f,(float)TILEGRID_WIDTH,(float)(TILEGRID_HEIGHT),0.f,-1.f,1.f)));
	vb->clear();

	// DRAW THE TILES
	if (EDITOR_LAYER == BACKGROUND_LAYER)
	{
		drawTiles(vb, false, 0xFFFFFFFF, true, ww::Rectanglei(viewX,viewY,TILEGRID_WIDTH/16,TILEGRID_HEIGHT/16));
		drawTiles(vb, true, 0x80FFFFFF, true, ww::Rectanglei(viewX,viewY,TILEGRID_WIDTH/16,TILEGRID_HEIGHT/16));
	}
	else if (EDITOR_LAYER == FOREGROUND_LAYER)
	{
		drawTiles(vb, false, 0x80FFFFFF, true, ww::Rectanglei(viewX,viewY,TILEGRID_WIDTH/16,TILEGRID_HEIGHT/16));
		drawTiles(vb, true, 0xFFFFFFFF, true, ww::Rectanglei(viewX,viewY,TILEGRID_WIDTH/16,TILEGRID_HEIGHT/16));
	}
	else if (EDITOR_LAYER == OBJECT_LAYER || EDITOR_LAYER == VIEW_LAYER)
	{
		drawTiles(vb, false, 0xFFFFFFFF, true, ww::Rectanglei(viewX,viewY,TILEGRID_WIDTH/16,TILEGRID_HEIGHT/16));
		drawTiles(vb, true, 0xFFFFFFFF, true, ww::Rectanglei(viewX,viewY,TILEGRID_WIDTH/16,TILEGRID_HEIGHT/16));
	}

	for(int i=0;i<leveldata->objects.size();i++)
	{
		if (sprTiles[leveldata->objects.at(i)->id] != NULL)
		{
			sprTiles[leveldata->objects.at(i)->id]->setPosition(16*leveldata->objects.at(i)->x-16*viewX,16*leveldata->objects.at(i)->y-16*viewY);
			vb->pushsprite(sprTiles[leveldata->objects.at(i)->id]);
		}
	}
	vb->update();
	vb->draw();
	ww::gfx::setMatrix(NULL,-1,glm::value_ptr(glm::ortho(0.f,(float)TILEGRID_WIDTH,(float)(TILEGRID_HEIGHT+64),64.f,-1.f,1.f)));
	vb->clear();
	// DRAW THE VIEWS
	static int TICKER = 0;
	TICKER++;

	for(unsigned int i=0;i<leveldata->views.size();i++)
	{
		unsigned int colors[6] = {0xFF0000, 0x00FF00, 0x0000FF, 0xFF00FF, 0x00FFFF, 0xFFFF00}; 
		ww::Rectanglei r = leveldata->views.at(i).view;
		r.x -= viewX;
		r.y -= viewY;
		r.x *= 16;
		r.y *= 16;
		r.width *= 16;
		r.height *= 16;
		r.y += 64;
		draw_rectangle(vb,r,colors[i%6] | ((EDITOR_LAYER==VIEW_LAYER?(int)(128+127*sin((TICKER + 2*i) * M_PI/60.f)):(int)(96+63*sin((TICKER + 2*i) * M_PI/60.f))) << 24),true,3);
	}
	if (EDITOR_LAYER == VIEW_LAYER && makingRectangle)
	{
		ww::Rectanglei r = tempRectangle;
		r.x -= viewX;
		r.y -= viewY;
		r.x *= 16;
		r.y *= 16;
		r.width *= 16;
		r.height *= 16;
		r.y += 64;

		if (tempRectangle.width < 16 || tempRectangle.height < 12 || leveldata->rectHitsView(tempRectangle))
		{
			if (TICKER/8 % 2 == 0)
				draw_rectangle(vb,r,0xFF0000FF,true,3);
			else
			{
				ww::Rectanglei r2 = tempRectangle;
				r2.x -= viewX;
				r2.y -= viewY;
				r2.x *= 16;
				r2.y *= 16;
				if (r2.width < 16)
					r2.width = 16;
				if (r2.height < 12)
					r2.height = 12;
				r2.width *= 16;
				r2.height *= 16;
				r2.y += 64;
				draw_rectangle(vb,r2,0xFF000000,true,3);
			}
		}
		else
			draw_rectangle(vb,r,0xFF000000,true,3);
	}
	vb->update();
	vb->draw();

	/*

				THE MENU


	*/


	ww::gfx::setRenderSubrect(ww::gfx::MakeRenderSubrect(0,64,ww::gfx::window_width,ww::gfx::window_height-64-16));
	ww::gfx::setMatrix(NULL,-1,glm::value_ptr(glm::ortho(0.f,(float)ww::gfx::window_width,(float)(ww::gfx::window_height-16),64.f,-1.f,1.f)));
	vb->clear();
	if (overlayHeight > 64)
	{
		draw_rectangle(vb, ww::Rectanglei(0, 64, ww::gfx::window_width, ww::gfx::window_height-64-16), 0xA0000000);

		if (EXTRAMODIFIER != NULL)
		{
			int mark4deletion = -1;
			for(int i=0;i<EXTRAMODIFIER->extradata.size();i++)
			{
				if (draw_button(vb,ww::Rectanglei(16,80 + 64*i,(3*(ww::gfx::window_width-208)/2)/4,48),EXTRAMODIFIER->extradata.at(i).name+((editingExtraName && i == editingExtraID)?"_":"")))
				{
					editingExtraName = true;
					editingExtraValue = false;
					ww::input::keyboard::keyboard_stringGM = EXTRAMODIFIER->extradata.at(i).name;
					editingExtraID = i;
					printf("WAHEY %i\n",editingExtraID);
				}
				if (draw_button(vb,ww::Rectanglei(16 + (3*(ww::gfx::window_width-208)/2)/4 + 16,80 + 64*i,80,48),EXTRAMODIFIER->extradata.at(i).istext?"STR":"NUM"))
					EXTRAMODIFIER->extradata.at(i).istext = !EXTRAMODIFIER->extradata.at(i).istext;
				std::string str = "";
				if (EXTRAMODIFIER->extradata.at(i).istext)
					str = EXTRAMODIFIER->extradata.at(i).text;
				else
				{
					sprintf(tempstring,"%i",EXTRAMODIFIER->extradata.at(editingExtraID).number);
					str = tempstring;
				}
				if (draw_button(vb,ww::Rectanglei(16 + (3*(ww::gfx::window_width-208)/2)/4 + 16 + 80 + 16,80 + 64*i,(5*(ww::gfx::window_width-208)/2)/4,48),str+((editingExtraValue && i == editingExtraID)?"_":"")))
				{
					editingExtraName = false;
					editingExtraValue = true;
					editingExtraID = i;

					if (EXTRAMODIFIER->extradata.at(i).istext)
						ww::input::keyboard::keyboard_stringGM = EXTRAMODIFIER->extradata.at(i).text;
					else
					{
						sprintf(tempstring,"%i",EXTRAMODIFIER->extradata.at(editingExtraID).number);
						ww::input::keyboard::keyboard_stringGM = tempstring;
					}
				}
				if (draw_button(vb,ww::Rectanglei(16 + (3*(ww::gfx::window_width-208)/2)/4 + 16 + 80 + 16 + (5*(ww::gfx::window_width-208)/2)/4 + 16,80 + 64*i,48,48),"X"))
				{
					mark4deletion = i;
				}
			}
			if (EXTRAMODIFIER->extradata.size() < 9)
			{
				if (draw_button(vb,ww::Rectanglei(16,ww::gfx::window_height-16-64,ww::gfx::window_width-32,48),"ADD MODIFIER"))
					EXTRAMODIFIER->extradata.push_back(ExtraData());
			}
			if (editingExtraName)
				EXTRAMODIFIER->extradata.at(editingExtraID).name = ww::input::keyboard::keyboard_stringGM;
			else if (editingExtraValue)
			{
				if (EXTRAMODIFIER->extradata.at(editingExtraID).istext)
					EXTRAMODIFIER->extradata.at(editingExtraID).text = ww::input::keyboard::keyboard_stringGM;
				else
				{
					EXTRAMODIFIER->extradata.at(editingExtraID).number = atoi(ww::input::keyboard::keyboard_stringGM.c_str());
					sprintf(tempstring,"%i",EXTRAMODIFIER->extradata.at(editingExtraID).number);
					ww::input::keyboard::keyboard_stringGM = tempstring;
				}
			}
			if (mark4deletion >= 0)
			{
				editingExtraName = false;
				editingExtraValue = false;
				editingExtraID = -1;
				EXTRAMODIFIER->extradata.erase(EXTRAMODIFIER->extradata.begin()+mark4deletion);
			}
		}
		else
		{
			if (EDITOR_LAYER == FOREGROUND_LAYER || EDITOR_LAYER == BACKGROUND_LAYER)
			{
				for(int i=TILE_NONE;i<TILE_TERMINATOR-1;i++)
				{
					if (draw_bubblebutton(vb,320+56*((i-TILE_NONE)%7),80+56*((i-TILE_NONE)/7),sprTiles[1+i]))//,(i+1 == CURRENT_TILE)?0xFFFFFFFF:0))
					{
						for(int j=0;j<recent_tiles.size();j++)
						{
							if (recent_tiles.at(j) == 1+i)
							{
								recent_tiles.erase(recent_tiles.begin()+j);
								j--;
							}
						}
						recent_tiles.insert(recent_tiles.begin(),1+i);
						while (recent_tiles.size() > 4)
							recent_tiles.erase(recent_tiles.begin()+4);
						CURRENT_TILE = 1+i;
					}
				}
			}
			else if (EDITOR_LAYER == OBJECT_LAYER)
			{
				for(int i=OBJECT_NONE;i<OBJECT_TERMINATOR-1;i++)
				{
					if (draw_bubblebutton(vb,320+56*((i-OBJECT_NONE)%7),80+56*((i-OBJECT_NONE)/7),sprTiles[1+i]))//,(i+1 == CURRENT_TILE)?0xFFFFFFFF:0))
					{
						for(int j=0;j<recent_objects.size();j++)
						{
							if (recent_objects.at(j) == 1+i)
							{
								recent_objects.erase(recent_objects.begin()+j);
								j--;
							}
						}
						recent_objects.insert(recent_objects.begin(),1+i);
						while (recent_objects.size() > 4)
							recent_objects.erase(recent_objects.begin()+4);
						CURRENT_OBJECT = 1+i;
					}
				}
			}


			if (draw_button(vb,ww::Rectanglei(16,80,288,48),editingFilename?(filename+"_"):filename))
			{
				editingFilename = !editingFilename;
				ww::input::keyboard::keyboard_stringGM = filename;
			}

			if (editingFilename)
				filename = ww::input::keyboard::keyboard_stringGM;
			if (draw_button(vb,ww::Rectanglei(16,144,288,48),"LOAD"))
			{
				leveldata->load(filename);
				for(int i=0;i<leveldata->width*leveldata->height;i++)
				{
					if (leveldata->foreground[i].id > 0 || leveldata->background[i].id > 0)
					{
						viewX = i % leveldata->width;
						viewY = i / leveldata->width;
						break;
					}
				}
			}
			if (draw_button(vb,ww::Rectanglei(16,208,288,48),"SAVE"))
				leveldata->save(filename);

		} // end EXTRAMODIFIER ISN'T NULL
	}
	else
	{
		if (ww::input::mouse::getY() >= 64)
		{
			if (ww::input::mouse::isButtonPressed(ww::input::mouse::MIDDLE))
			{
				dragStartX = ww::input::mouse::getX();
				dragStartY = ww::input::mouse::getY();
				viewStartX = viewX;
				viewStartY = viewY;
				dragging = true;
			}
		}
		if (dragging)
		{
			viewX = viewStartX - (ww::input::mouse::getX() - dragStartX)/16;
			viewY = viewStartY - (ww::input::mouse::getY() - dragStartY)/16;
			if (ww::input::mouse::isButtonReleased(ww::input::mouse::MIDDLE))
				dragging = false;
		}
		else
		{
			if (ww::input::mouse::hits(ww::Rectanglei(0,64,ww::gfx::window_width,ww::gfx::window_height-16-64)))
			{
				int tileX = viewX+ww::input::mouse::getX()/16;
				int tileY = viewY+(ww::input::mouse::getY()-64)/16;
				if (EDITOR_LAYER == VIEW_LAYER)
				{
					if (makingRectangle)
					{
						//printf("TEMPRECT: %i %i %i %i\n",tempRectangle.x,tempRectangle.y,tempRectangle.width,tempRectangle.height);
						ww::Rectanglei b4 = tempRectangle;
						tempRectangle.x = min(rectangleX,tileX);
						tempRectangle.y = min(rectangleY,tileY);
						tempRectangle.width = 1+abs(rectangleX - tileX);
						tempRectangle.height = 1+abs(rectangleY- tileY);
						//if (leveldata->rectHitsView(tempRectangle))
						//	tempRectangle = b4;

						if (ww::input::mouse::hits(ww::Rectanglei(ww::gfx::window_width-16,64,16,ww::gfx::window_height-16-64)))
							viewX += 1;
						if (ww::input::mouse::hits(ww::Rectanglei(0,64,16,ww::gfx::window_height-16-64)))
							viewX -= 1;

						if (ww::input::mouse::isButtonReleased(ww::input::mouse::LEFT))
						{
							makingRectangle = false;
							if (!leveldata->rectHitsView(tempRectangle) && tempRectangle.width >= 16 && tempRectangle.height >= 12)
							{
								View v;
								v.extra = NULL;
								v.view = tempRectangle;
								leveldata->views.push_back(v);
							}
						}
					}
					else
					{
						if (ww::input::mouse::isButtonPressed(ww::input::mouse::RIGHT))
						{
							if (ww::input::keyboard::isKeyDown(ww::input::key::LShift) || ww::input::keyboard::isKeyDown(ww::input::key::RShift))
							{
								if (leveldata->pointHitsView(tileX,tileY))
								{
									ww::Rectanglei pt(tileX,tileY,1,1);
									bool go = true;
									for(unsigned int i=0;i<leveldata->views.size() && go;i++)
									{
										if (leveldata->views.at(i).view.intersects(pt))
										{
											if (leveldata->views.at(i).extra != NULL)
											{
												delete leveldata->views.at(i).extra;
												leveldata->views.at(i).extra = NULL;
											}
											leveldata->views.erase(leveldata->views.begin() + i);
											go = false;
										}
									}
								}
							}
							else					// MODIFY THE EXTRA 
							{
								if (leveldata->pointHitsView(tileX,tileY))
								{
									ww::Rectanglei pt(tileX,tileY,1,1);
									bool go = true;
									for(unsigned int i=0;i<leveldata->views.size() && go;i++)
									{
										if (leveldata->views.at(i).view.intersects(pt))
										{
											/*if (leveldata->views.at(i).extra != NULL)
											{
												delete leveldata->views.at(i).extra;
												leveldata->views.at(i).extra = NULL;
											}
											leveldata->views.erase(leveldata->views.begin() + i);*/
											if (leveldata->views.at(i).extra == NULL)
												leveldata->views.at(i).extra = new Extra();
											EXTRAMODIFIER = leveldata->views.at(i).extra;
											editingExtraName = false;
											editingExtraValue = false;
											go = false;
										}
									}
								}								
							}
						}
					}
					if (ww::input::mouse::isButtonPressed(ww::input::mouse::LEFT))
					{
						if (!leveldata->pointHitsView(tileX,tileY))
						{
							makingRectangle = true;
							rectangleX = tileX;
							rectangleY = tileY;
							tempRectangle.x = tileX;
							tempRectangle.y = tileY;
							tempRectangle.width = 1;
							tempRectangle.height = 1;
						}
					}
				}
				else 
				{
					if (tileX >= 0 && tileY >= 0)
					{
						if (EDITOR_LAYER == BACKGROUND_LAYER || EDITOR_LAYER == FOREGROUND_LAYER)
						{
							if (ww::input::mouse::isButtonDown(ww::input::mouse::LEFT))
							{
								if (EDITOR_LAYER == BACKGROUND_LAYER)
									leveldata->getBackgroundTile(tileX,tileY)->id = CURRENT_TILE;
								else if (EDITOR_LAYER == FOREGROUND_LAYER)
									leveldata->getForegroundTile(tileX,tileY)->id = CURRENT_TILE;
							}
							if (ww::input::mouse::isButtonDown(ww::input::mouse::RIGHT) && (ww::input::keyboard::isKeyDown(ww::input::key::LShift) || ww::input::keyboard::isKeyDown(ww::input::key::RShift)))
							{
								if (EDITOR_LAYER == BACKGROUND_LAYER)
									leveldata->getBackgroundTile(tileX,tileY)->id = TILE_NONE;
								else if (EDITOR_LAYER == FOREGROUND_LAYER)
								{
									leveldata->getForegroundTile(tileX,tileY)->id = TILE_NONE;
									if (leveldata->getForegroundTile(tileX,tileY)->extra != NULL)
									{
										delete leveldata->getForegroundTile(tileX,tileY)->extra;
										leveldata->getForegroundTile(tileX,tileY)->extra = NULL;
									}
								}
							}
							else if (ww::input::mouse::isButtonPressed(ww::input::mouse::RIGHT))
							{
								if (EDITOR_LAYER == FOREGROUND_LAYER)
								{
									if (leveldata->getForegroundTile(tileX,tileY)->id == TILE_VOICEBOX)
									{
										if (leveldata->getForegroundTile(tileX,tileY)->extra == NULL)
											leveldata->getForegroundTile(tileX,tileY)->extra = new Extra();
										EXTRAMODIFIER = leveldata->getForegroundTile(tileX,tileY)->extra;
										editingExtraName = false;
										editingExtraValue = false;
									}
								}
							}
						}
						else if (EDITOR_LAYER == OBJECT_LAYER)
						{
							if (ww::input::mouse::isButtonPressed(ww::input::mouse::LEFT))
							{
								Object *obj = new Object();
								obj->id = CURRENT_OBJECT;
								obj->x = tileX;
								obj->y = tileY;
								leveldata->objects.push_back(obj);
							}
							else if (ww::input::mouse::isButtonDown(ww::input::mouse::RIGHT) && (ww::input::keyboard::isKeyDown(ww::input::key::LShift) || ww::input::keyboard::isKeyDown(ww::input::key::RShift)))
							{
								for(int i=0;i<leveldata->objects.size();i++)
								{
									ww::Rectanglei pt(16*tileX,16*tileY,16,16);
									ww::vec2dui s(16,16);
									if (sprTiles[leveldata->objects.at(i)->id] != NULL)
										s = sprTiles[leveldata->objects.at(i)->id]->getSize();
									ww::Rectanglei obj(16*leveldata->objects.at(i)->x,16*leveldata->objects.at(i)->y,s.x,s.y);
									if (pt.intersects(obj))
									{
										delete leveldata->objects.at(i);
										leveldata->objects.erase(leveldata->objects.begin()+i);
										i--;
									}
								}
							}
						}
					}
				}
			}
		}
	}
	vb->update();
	vb->draw();

	/*

											STATUS BAR

	*/

	ww::gfx::setRenderSubrect(ww::gfx::MakeRenderSubrect(0,ww::gfx::window_height-16,ww::gfx::window_width,16));
	ww::gfx::setMatrix(NULL,-1,glm::value_ptr(glm::ortho(0.f,(float)ww::gfx::window_width,16.f,0.f,-1.f,1.f)));
	vb->clear();
	// poop
	draw_rectangle(vb, ww::Rectanglei(0, 0, ww::gfx::window_width, 16),0x80000000);
	char text[10];
	memset(text,0,10);
	sprintf(text,"X: %i",viewX);
	draw_text(vb,4,4,text,1);
	memset(text,0,10);
	sprintf(text,"Y: %i",viewY);
	draw_text(vb,84,4,text,1);
	vb->update();
	vb->draw();
}