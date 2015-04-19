#ifndef __WADEMCGILLISLD32__LevelData_h__
#define __WADEMCGILLISLD32__LevelData_h__
#include <string>
#include <vector>
#include <WadeWork/types.h>

const unsigned short TILE_NONE = 0;
const unsigned short TILE_DIRT = 1;
const unsigned short TILE_GRASS = 2;
const unsigned short TILE_FORESTDIRT = 3;
const unsigned short TILE_FORESTGRASS = 4;
const unsigned short TILE_LEAVES = 5;
const unsigned short TILE_STUMP = 6;
const unsigned short TILE_BRIDGE = 7;
const unsigned short TILE_CHAIN_NE = 8;
const unsigned short TILE_BRICK = 9;
const unsigned short TILE_SMALLBRICK = 10;
const unsigned short TILE_WOOD = 11;
const unsigned short TILE_SHRINE_LEFT = 12;
const unsigned short TILE_SHRINE_RIGHT = 13;
const unsigned short TILE_VOICEBOX = 14;
const unsigned short TILE_LAVA = 15;
const unsigned short TILE_CHAIN_NW = 16;
const unsigned short TILE_REDSMALLBRICK = 17;
const unsigned short TILE_PURPLESMALLBRICK = 18;
const unsigned short TILE_REDBRICK = 19;
const unsigned short TILE_PURPLEBRICK = 20;
const unsigned short TILE_JUNGLEGROUND = 21;
const unsigned short TILE_JUNGLEBACKGROUND = 22;
const unsigned short TILE_VINE = 23;
const unsigned short TILE_VINEORB = 24;
const unsigned short TILE_REDDARKGROUND = 25;
const unsigned short TILE_TERMINATOR = 26;

const unsigned short OBJECT_NONE = 64;
const unsigned short OBJECT_PLAYER = 65;
const unsigned short OBJECT_POWERUP_FISH = 66;
const unsigned short OBJECT_CRAWLER = 67;
const unsigned short OBJECT_POWERUP_TOASTER = 68;
const unsigned short OBJECT_POWERUP_APPLE = 69;
const unsigned short OBJECT_TERMINATOR = 70;

typedef struct {
	std::string name;
	bool istext;
	int number;
	std::string text;
} ExtraData;

typedef struct {
	std::vector<ExtraData> extradata;
} Extra;

typedef struct {
	unsigned short id;
	Extra *extra;
} Tile;

typedef struct {
	unsigned short id;
	unsigned short x;
	unsigned short y;
	Extra *extra;
} Object;

class LevelData
{
public:
	unsigned short width;
	unsigned short height;
	Tile *background;
	Tile *foreground;
	std::vector<Object *> objects;
	std::vector<ww::Rectanglei> views;
	//
public:
	bool load(std::string path);
	bool save(std::string path);
public:
	void wipe();
	void wipeObjects();
	void reset(unsigned short width, unsigned short height);
	Tile *getForegroundTile(unsigned short x, unsigned short y);
	Tile *getBackgroundTile(unsigned short x, unsigned short y);

	bool pointHitsView(unsigned short x, unsigned short y);
	bool rectHitsView(ww::Rectanglei rect);

	void fwriteExtra(Extra *extra, FILE *handle);
	void freadExtra(Extra *extra, FILE *handle);
};

extern std::string readExtraString(Extra *extra, std::string name);
extern int readExtraNumber(Extra *extra, std::string name);

extern void writeExtraString(Extra *extra, std::string name, std::string text);
extern void writeExtraNumber(Extra *extra, std::string name, int number);
#endif