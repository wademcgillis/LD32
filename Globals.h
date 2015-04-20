#ifndef __WADEMCGILLISLD32__Globals_h__
#define __WADEMCGILLISLD32__Globals_h__
#include "LevelData.h"
#include <WadeWork/wadework.h>
#define M_PI 3.1415926535f
namespace ww
{
	namespace gfx
	{
		extern unsigned int window_width;
		extern unsigned int window_height;
	}
	namespace input
	{
		namespace keyboard
		{
			extern std::string keyboard_stringGM;
		}
	}
}

const unsigned char VIEW_LAYER = 1;
const unsigned char OBJECT_LAYER = 2;
const unsigned char FOREGROUND_LAYER = 3;
const unsigned char BACKGROUND_LAYER = 4;

#ifndef MAIN
#define GLOBAL extern
#else
#define GLOBAL
#endif

GLOBAL unsigned char EDITOR_LAYER;
GLOBAL unsigned short CURRENT_TILE;
GLOBAL unsigned short CURRENT_OBJECT;

GLOBAL bool IN_EDITOR;

GLOBAL bool RELEASE;


GLOBAL std::vector<ww::sfx::Sound*> allSounds;
GLOBAL bool soundEnabled;
GLOBAL void setSoundEnabled(bool enable);
GLOBAL void LD32LoadSound(ww::sfx::Sound *sound, std::string path);
GLOBAL void LD32PlaySoundWarbled(ww::sfx::Sound *sound, float factor = 0.1f);

GLOBAL ww::gfx::Texture *tex;
GLOBAL ww::gfx::Sprite *highlightybob[2];
GLOBAL ww::gfx::Sprite *font[64];
GLOBAL ww::gfx::Sprite *sprTiles[128];
GLOBAL ww::gfx::Sprite *rectangle;
GLOBAL ww::gfx::Sprite *sprForeground, *sprObjects, *sprBackground, *sprView, *sprPlay;
GLOBAL ww::gfx::Sprite *sprMessagebox[9];
GLOBAL ww::gfx::Sprite *sprPowerupOrb[4];
GLOBAL ww::gfx::Sprite *sprFishWeapon[2];
GLOBAL ww::gfx::Sprite *sprToasterWeapon;
GLOBAL ww::gfx::Sprite *sprAppleWeapon;
GLOBAL ww::gfx::Sprite *sprSpeedWeapon;
GLOBAL ww::gfx::Sprite *sprToast;
GLOBAL ww::gfx::Sprite *sprPlayerWhite;
GLOBAL ww::gfx::Sprite *sprPlayerBurnt;
GLOBAL ww::gfx::Sprite *sprHUDHeart[2];
GLOBAL ww::gfx::Sprite *sprCrawler[4];
GLOBAL ww::gfx::Sprite *sprCrawlerPop[2];
GLOBAL ww::gfx::Sprite *sprLava[8];
GLOBAL ww::gfx::Sprite *sprDelilah[6];
GLOBAL ww::gfx::Sprite *sprBlueSpinyRodent[4];
GLOBAL ww::gfx::Sprite *sprPlayerSparkle[2];
GLOBAL ww::gfx::Sprite *sprCrawlerSparkle[2];
GLOBAL ww::gfx::Sprite *sprBlueSpinyRodentSparkle[2];
GLOBAL ww::gfx::Sprite *sprSpecialSwitch[2];
GLOBAL ww::gfx::Sprite *sprLogo;

GLOBAL bool hasFishWeapon;
GLOBAL bool hasToasterWeapon;
GLOBAL bool hasAppleWeapon;
GLOBAL bool hasSpeedWeapon;

GLOBAL unsigned int powerupCountdown;


GLOBAL ww::gfx::VertexBatch *vb;
GLOBAL LevelData *leveldata;
GLOBAL std::vector<unsigned short> recent_tiles;
GLOBAL std::vector<unsigned short> recent_objects;
GLOBAL int viewX;
GLOBAL int viewY;
GLOBAL int viewStartX;
GLOBAL int viewStartY;
GLOBAL bool dragging;
GLOBAL int dragStartX;
GLOBAL int dragStartY;
GLOBAL int rectangleX;
GLOBAL int rectangleY;
GLOBAL bool makingRectangle;
GLOBAL ww::Rectanglei tempRectangle;
GLOBAL bool editingFilename;
GLOBAL std::string filename;

GLOBAL Extra *EXTRAMODIFIER;
GLOBAL bool editingExtraName;
GLOBAL int editingExtraID;
GLOBAL bool editingExtraValue;

GLOBAL char tempstring[256];




GLOBAL void draw_rectangle(ww::gfx::VertexBatch *vb, ww::Rectanglei rect, unsigned int color, bool outline = false, int thickness = 1);
GLOBAL void draw_text(ww::gfx::VertexBatch *vb, int x, int y, std::string text, unsigned int scale, unsigned int color = 0xFFFFFFFF);
GLOBAL bool draw_button(ww::gfx::VertexBatch *vb, ww::Rectanglei rect, std::string text);
GLOBAL bool draw_bubblebutton(ww::gfx::VertexBatch *vb, int x, int y, ww::gfx::Sprite *spr, unsigned int color = 0);
GLOBAL ww::gfx::Sprite *MakeTileSprite(ww::gfx::Texture *texture, int x, int y, unsigned int width = 16, unsigned int height = 16);
GLOBAL void drawTiles(ww::gfx::VertexBatch *vb, bool foreground, unsigned int color, bool editor, ww::Rectanglei rect);
#endif