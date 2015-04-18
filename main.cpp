#include <WadeWork/wadework.h>
#include "LevelData.h"

bool doinit();
bool dorun();
bool doend();

void doGame();
void doEditor();
bool IN_EDITOR = true;

const unsigned char VIEW_LAYER = 1;
const unsigned char OBJECT_LAYER = 2;
const unsigned char FOREGROUND_LAYER = 3;
const unsigned char BACKGROUND_LAYER = 4;
unsigned char EDITOR_LAYER;
unsigned short CURRENT_TILE = -1;

int main()
{
	ww::gfx::setWindowSize(256*3,224*3);
	ww::sys::setInitCallback(doinit);
	ww::sys::setTimerCallback(dorun);
	ww::sys::setDeinitCallback(doend);
	ww::sys::setTimerResolution(60);
	ww::sys::setup(ww::sys::CONFIG_OPENGL1 | ww::sys::CONFIG_DISABLE_OPENAL | ww::sys::CONFIG_DISABLE_OPENGL_DEPTHBUFFER);
}

ww::gfx::Texture *tex;
ww::gfx::Sprite *highlightybob[2];
ww::gfx::Sprite *font[64];
ww::gfx::Sprite *sprTiles[64];
ww::gfx::Sprite *rectangle;
ww::gfx::Sprite *sprForeground, *sprObjects, *sprBackground, *sprView, *sprPlay;
ww::gfx::VertexBatch *vb;
LevelData *leveldata;
std::vector<unsigned short> recent_tiles;

namespace ww
{
	namespace gfx
	{
		extern unsigned int window_width;
		extern unsigned int window_height;
	}
}

void draw_rectangle(ww::gfx::VertexBatch *vb, ww::Rectanglei rect, unsigned int color)
{
	rectangle->setSize(rect.width,rect.height);
	rectangle->setColor(color);
	rectangle->setPosition(rect.x,rect.y);
	vb->pushsprite(rectangle);
}

void draw_text(ww::gfx::VertexBatch *vb, int x, int y, std::string text, unsigned int scale, unsigned int color = 0xFFFFFFFF)
{
	int xstart = x-8*scale, ystart = y;
	x = xstart;
	int l = text.length();
	unsigned char c;
	for(int i=0;i<l;i++)
	{
		c = text.at(i);
		if (c == '\n')
		{
			x = xstart;
			y += 8*scale;
			continue;
		}
		x += 8*scale;
		if (c >= 'a' && c <= 'z')
			c -= 'a'-'A';
		if (c > '_')
			continue;
		else if (c == ' ')
			continue;
		else
		{
			font[c-32]->setPosition(x,y);
			font[c-32]->setColor(color);
			font[c-32]->setSize(8*scale,8*scale);
			vb->pushsprite(font[c-32]);
		}
	}
}

bool draw_button(ww::gfx::VertexBatch *vb, ww::Rectanglei rect, std::string text)
{
	int w = 2 * 8 * text.length();
	int xx = (rect.width - w)/2;
	int yy = (rect.height - 16)/2;
	bool hover =  ww::input::mouse::hits(rect);
	draw_rectangle(vb, rect, hover?0x80A0A0A0:0x80000000);
	draw_text(vb,rect.x + xx, rect.y + yy,text,2);
	return hover && ww::input::mouse::isButtonPressed(ww::input::mouse::LEFT);
}

bool draw_bubblebutton(ww::gfx::VertexBatch *vb, int x, int y, ww::gfx::Sprite *spr, unsigned int color = 0)
{
	bool hover = ww::input::mouse::hits(ww::Rectanglei(x,y,48,48));

	highlightybob[1]->setColor(hover?0x80404040:0x80000000);
	highlightybob[1]->setPosition(x,y);
	vb->pushsprite(highlightybob[1]);

	if (spr != NULL)
	{
		spr->setPosition(x+8,y+8);
		spr->setSize(32,32);
		vb->pushsprite(spr);
	}
	
	if (hover)
		highlightybob[0]->setColor(color?color:0xFFFFFFFF);
	else
		highlightybob[0]->setColor(color?color:0xFFA0A0A0);
	highlightybob[0]->setPosition(x,y);
	vb->pushsprite(highlightybob[0]);

	return hover && ww::input::mouse::isButtonPressed(ww::input::mouse::LEFT);
}


ww::gfx::Sprite *MakeTileSprite(ww::gfx::Texture *texture, int x, int y, unsigned int width = 16, unsigned int height = 16)
{
	ww::gfx::Sprite *spr = new ww::gfx::Sprite();
	spr->setSize(16,16);
	spr->setPosition(0,0);
	spr->setSubimageUVRect(0,tex->getUVRectFromIntRect(ww::Rectanglei(x,y,width,height)));
	spr->setColor(0xFFFFFFFF);
	return spr;
}

bool doinit()
{
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
	sprTiles[TILE_CHAIN] = MakeTileSprite(tex,96,0);
	sprTiles[TILE_LEAVES] = MakeTileSprite(tex,96,16);
	sprTiles[TILE_WOOD] = MakeTileSprite(tex,112,0);
	sprTiles[TILE_STUMP] = MakeTileSprite(tex,112,16);

	sprPlay = MakeTileSprite(tex,160,224,32,32);
	sprView = MakeTileSprite(tex,192,224,32,32);
	sprObjects = MakeTileSprite(tex,64,192);
	sprForeground = MakeTileSprite(tex,80,192);
	sprBackground = MakeTileSprite(tex,96,192);

	leveldata = new LevelData();
	leveldata->reset(1024,1024);

	EDITOR_LAYER = OBJECT_LAYER;

	return true;
}

void editorDrawTiles(ww::gfx::VertexBatch *vb, bool foreground, unsigned int color, ww::Rectanglei rect)
{
	Tile *tile = NULL;
	for(int i=rect.x;i<rect.x+rect.width;i++)
		for(int j=rect.y;j<rect.y+rect.height;j++)
		{
			if (i < 0 || j < 0)
				continue;
			if (foreground)
				tile = leveldata->getForegroundTile(i,j);
			else
				tile = leveldata->getBackgroundTile(i,j);
			if (tile->id == TILE_NONE)
				continue;
			if (sprTiles[tile->id] != NULL)
			{
				sprTiles[tile->id]->setSize(16,16);
				sprTiles[tile->id]->setPosition(16*(i-rect.x),64+16*(j-rect.y));
				sprTiles[tile->id]->setColor(color);
				vb->pushsprite(sprTiles[tile->id]);
			}
		}
}

int viewX = 0;
int viewY = 0;
int viewStartX = 0;
int viewStartY = 0;
bool dragging = false;
int dragStartX = 0;
int dragStartY = 0;
void doEditor()
{
	glClearColor(1.f,1.f,1.f,1.f);
	glClear(GL_COLOR_BUFFER_BIT);
	ww::gfx::setMatrix(NULL,-1,glm::value_ptr(glm::ortho(0.f,(float)ww::gfx::window_width,(float)ww::gfx::window_height,0.f,-1.f,1.f)));
	tex->bind();
	vb->clear();


	if (EDITOR_LAYER == BACKGROUND_LAYER)
	{
		editorDrawTiles(vb, false, 0xFFFFFFFF, ww::Rectanglei(viewX,viewY,ww::gfx::window_width/16,(ww::gfx::window_height-64)/16));
		editorDrawTiles(vb, true, 0x80FFFFFF, ww::Rectanglei(viewX,viewY,ww::gfx::window_width/16,(ww::gfx::window_height-64)/16));
	}
	else if (EDITOR_LAYER == FOREGROUND_LAYER)
	{
		editorDrawTiles(vb, false, 0x80FFFFFF, ww::Rectanglei(viewX,viewY,ww::gfx::window_width/16,(ww::gfx::window_height-64)/16));
		editorDrawTiles(vb, true, 0xFFFFFFFF, ww::Rectanglei(viewX,viewY,ww::gfx::window_width/16,(ww::gfx::window_height-64)/16));
	}
	else if (EDITOR_LAYER == OBJECT_LAYER)
	{
		editorDrawTiles(vb, false, 0xFFFFFFFF, ww::Rectanglei(viewX,viewY,ww::gfx::window_width/16,(ww::gfx::window_height-64)/16));
		editorDrawTiles(vb, true, 0xFFFFFFFF, ww::Rectanglei(viewX,viewY,ww::gfx::window_width/16,(ww::gfx::window_height-64)/16));
	}

	static int overlayHeight = 64;
	draw_rectangle(vb, ww::Rectanglei(0, 0, ww::gfx::window_width, 64), 0xA0000000);
	if (overlayHeight > 64)
		draw_rectangle(vb, ww::Rectanglei(0, 0, ww::gfx::window_width, ww::gfx::window_height), 0xA0000000);

	draw_text(vb,456,24,"RECENTLY\n  USED",1);
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

	draw_bubblebutton(vb,8,8,sprPlay,0xFF0000FF);
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
		if (draw_button(vb,ww::Rectanglei(312,8,128,48),"LESS"))
			overlayHeight = 64;


		for(int i=0;i<TILE_WOOD;i++)
		{
			if (draw_bubblebutton(vb,320+56*(i%7),80+56*(i/7),sprTiles[1+i]))//,(i+1 == CURRENT_TILE)?0xFFFFFFFF:0))
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
					recent_tiles.erase(recent_tiles.begin()+5);
				CURRENT_TILE = 1+i;
			}
		}
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
			if (draw_button(vb,ww::Rectanglei(312,8,128,48),"MORE"))
				overlayHeight = ww::gfx::window_height;

			if (ww::input::mouse::hits(ww::Rectanglei(0,64,ww::gfx::window_width,ww::gfx::window_height-16-64)))
			{
				int tileX = viewX+ww::input::mouse::getX()/16;
				int tileY = viewY+(ww::input::mouse::getY()-64)/16;
				if (tileX >= 0 && tileY >= 0)
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
							leveldata->getForegroundTile(tileX,tileY)->id = TILE_NONE;
					}
				}
			}
		}
	}


	// poop
	draw_rectangle(vb, ww::Rectanglei(0, ww::gfx::window_height-16, ww::gfx::window_width, 16),0x80000000);
	char text[10];
	memset(text,0,10);
	sprintf(text,"X: %i",viewX);
	draw_text(vb,4,ww::gfx::window_height-12,text,1);
	memset(text,0,10);
	sprintf(text,"Y: %i",viewY);
	draw_text(vb,84,ww::gfx::window_height-12,text,1);
	vb->update();
	vb->draw();
}

void doGame()
{

}



bool dorun()
{
	if (IN_EDITOR)
		doEditor();
	else
		doGame();
	return true;
}

bool doend()
{
	return true;
}