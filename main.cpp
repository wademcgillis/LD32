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
unsigned short CURRENT_TILE = TILE_NONE;
unsigned short CURRENT_OBJECT = OBJECT_NONE;

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
ww::gfx::Sprite *sprTiles[128];
ww::gfx::Sprite *rectangle;
ww::gfx::Sprite *sprForeground, *sprObjects, *sprBackground, *sprView, *sprPlay;
ww::gfx::VertexBatch *vb;
LevelData *leveldata;
std::vector<unsigned short> recent_tiles;
std::vector<unsigned short> recent_objects;
int viewX = 0;
int viewY = 0;
int viewStartX = 0;
int viewStartY = 0;
bool dragging = false;
int dragStartX = 0;
int dragStartY = 0;
int rectangleX = 0;
int rectangleY = 0;
bool makingRectangle = false;
ww::Rectanglei tempRectangle;
bool editingFilename = false;
std::string filename = "";

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

void draw_rectangle(ww::gfx::VertexBatch *vb, ww::Rectanglei rect, unsigned int color, bool outline = false, int thickness = 1)
{
	if (outline)
	{
		// left
		rectangle->setSize(thickness,rect.height);
		rectangle->setColor(color);
		rectangle->setPosition(rect.x,rect.y);
		vb->pushsprite(rectangle);
		// right
		rectangle->setSize(thickness,rect.height);
		rectangle->setColor(color);
		rectangle->setPosition(rect.x+rect.width-thickness,rect.y);
		vb->pushsprite(rectangle);
		// top
		rectangle->setSize(rect.width-thickness*2,thickness);
		rectangle->setColor(color);
		rectangle->setPosition(rect.x+thickness,rect.y);
		vb->pushsprite(rectangle);
		// bottom
		rectangle->setSize(rect.width-thickness*2,thickness);
		rectangle->setColor(color);
		rectangle->setPosition(rect.x+thickness,rect.y+rect.height-thickness);
		vb->pushsprite(rectangle);
	}
	else
	{
		rectangle->setSize(rect.width,rect.height);
		rectangle->setColor(color);
		rectangle->setPosition(rect.x,rect.y);
		vb->pushsprite(rectangle);
	}
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

	sprTiles[OBJECT_NONE] = MakeTileSprite(tex,0,0,256,256);
	sprTiles[OBJECT_NONE]->setSize(64,64);
	sprTiles[OBJECT_PLAYER] = MakeTileSprite(tex,0,32,32,32);

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

void drawTiles(ww::gfx::VertexBatch *vb, bool foreground, unsigned int color, bool editor, ww::Rectanglei rect)
{
	Tile *tile = NULL;
	for(int i=rect.x;i<rect.x+rect.width;i++)
		for(int j=rect.y;j<rect.y+rect.height;j++)
		{
			if (i < 0 || j < 0 || i >= leveldata->width || j >= leveldata->height)
				continue;
			if (foreground)
				tile = leveldata->getForegroundTile(i,j);
			else
				tile = leveldata->getBackgroundTile(i,j);
			if (tile->id == TILE_NONE)
				continue;
			if (tile->id > 15)
			{
				printf("BAD TILE ID! %i, %i : %i\n",i,j,tile->id);
				continue;
			}
			if (sprTiles[tile->id] != NULL)
			{
				sprTiles[tile->id]->setSize(16,16);
				if (editor)
					sprTiles[tile->id]->setPosition(16*(i-rect.x),16*(j-rect.y));
				else
					sprTiles[tile->id]->setPosition(16*i,16*j);
				sprTiles[tile->id]->setColor(color);
				vb->pushsprite(sprTiles[tile->id]);
			}
		}
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
		IN_EDITOR = false;
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
	}
	else
	{
		if (draw_button(vb,ww::Rectanglei(312,8,128,48),"MORE"))
		{
			overlayHeight = ww::gfx::window_height;
			editingFilename = false;
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
		sprTiles[leveldata->objects.at(i)->id]->setPosition(16*leveldata->objects.at(i)->x-16*viewX,16*leveldata->objects.at(i)->y-16*viewY);
		vb->pushsprite(sprTiles[leveldata->objects.at(i)->id]);
	}
	vb->update();
	vb->draw();
	ww::gfx::setMatrix(NULL,-1,glm::value_ptr(glm::ortho(0.f,(float)TILEGRID_WIDTH,(float)(TILEGRID_HEIGHT+64),64.f,-1.f,1.f)));
	vb->clear();
	// DRAW THE VIEWS
	static int TICKER = 0;
	TICKER++;
#define M_PI 3.1415926535f
	for(unsigned int i=0;i<leveldata->views.size();i++)
	{
		unsigned int colors[6] = {0xFF0000, 0x00FF00, 0x0000FF, 0xFF00FF, 0x00FFFF, 0xFFFF00}; 
		ww::Rectanglei r = leveldata->views.at(i);
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

		if (tempRectangle.width < 16 || tempRectangle.height < 14 || leveldata->rectHitsView(tempRectangle))
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
				if (r2.height < 14)
					r2.height = 14;
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


	ww::gfx::setRenderSubrect(ww::gfx::MakeRenderSubrect(0,64,ww::gfx::window_width,ww::gfx::window_height-64-16));
	ww::gfx::setMatrix(NULL,-1,glm::value_ptr(glm::ortho(0.f,(float)ww::gfx::window_width,(float)(ww::gfx::window_height-16),64.f,-1.f,1.f)));
	vb->clear();
	if (overlayHeight > 64)
	{
		draw_rectangle(vb, ww::Rectanglei(0, 64, ww::gfx::window_width, ww::gfx::window_height-64-16), 0xA0000000);
		if (EDITOR_LAYER == FOREGROUND_LAYER || EDITOR_LAYER == BACKGROUND_LAYER)
		{
			for(int i=TILE_NONE;i<TILE_WOOD;i++)
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
			for(int i=OBJECT_NONE;i<OBJECT_PLAYER;i++)
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
			leveldata->load(filename);
		if (draw_button(vb,ww::Rectanglei(16,208,288,48),"SAVE"))
			leveldata->save(filename);
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
							if (!leveldata->rectHitsView(tempRectangle) && tempRectangle.width >= 16 && tempRectangle.height >= 14)
							{
								leveldata->views.push_back(tempRectangle);
							}
						}
					}
					else
					{
						if (ww::input::mouse::isButtonPressed(ww::input::mouse::RIGHT))
						{
							if (leveldata->pointHitsView(tileX,tileY))
							{
								ww::Rectanglei pt(tileX,tileY,1,1);
								bool go = true;
								for(unsigned int i=0;i<leveldata->views.size() && go;i++)
								{
									if (leveldata->views.at(i).intersects(pt))
									{
										leveldata->views.erase(leveldata->views.begin() + i);
										go = false;
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
									leveldata->getForegroundTile(tileX,tileY)->id = TILE_NONE;
							}
						}
						else if (EDITOR_LAYER == OBJECT_LAYER)
						{
							if (ww::input::mouse::isButtonDown(ww::input::mouse::LEFT))
							{
								Object *obj = new Object();
								obj->id = CURRENT_OBJECT;
								obj->x = tileX;
								obj->y = tileY;
								leveldata->objects.push_back(obj);
							}
							if (ww::input::mouse::isButtonDown(ww::input::mouse::RIGHT) && (ww::input::keyboard::isKeyDown(ww::input::key::LShift) || ww::input::keyboard::isKeyDown(ww::input::key::RShift)))
							{
								for(int i=0;i<leveldata->objects.size();i++)
								{
									ww::Rectanglei pt(16*tileX,16*tileY,16,16);
									ww::Rectanglei obj(16*leveldata->objects.at(i)->x,16*leveldata->objects.at(i)->y,sprTiles[leveldata->objects.at(i)->id]->getSize().x,sprTiles[leveldata->objects.at(i)->id]->getSize().y);
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

void doGame()
{
	static ww::Rectanglei viewRect(0,0,256,224);
	ww::gfx::setRenderSubrect(ww::gfx::MakeRenderSubrect(0,0,ww::gfx::window_width,ww::gfx::window_height));
	glClearColor(1.f,1.f,1.f,1.f);
	glClear(GL_COLOR_BUFFER_BIT);
	tex->bind();

	static int px = leveldata->objects.at(1)->x;
	static int py = leveldata->objects.at(1)->y;
	if (ww::input::keyboard::isKeyDown(ww::input::key::Left))
		px -= 4;
	if (ww::input::keyboard::isKeyDown(ww::input::key::Right))
		px += 4;
	if (ww::input::keyboard::isKeyDown(ww::input::key::Up))
		py -= 4;
	if (ww::input::keyboard::isKeyDown(ww::input::key::Down))
		py += 4;


	bool inAView = false;
	ww::Rectanglei inViewRect;
	ww::Rectanglei testRect(px+16,py+16,1,1);
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
	viewRect.x = px+16-256/2;
	viewRect.y = py+16-224/2;
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
	

	ww::gfx::setMatrix(NULL,-1,glm::value_ptr(glm::ortho((float)viewRect.x,(float)(viewRect.x+viewRect.width),(float)(viewRect.y+viewRect.height),(float)viewRect.y,-1.f,1.f)));
	vb->clear();

	drawTiles(vb, false, 0xFFFFFFFF, false, ww::Rectanglei(-2+viewRect.x/16,-2+viewRect.y/16,viewRect.width/16+4,viewRect.height/16+4));
	drawTiles(vb, true, 0xFFFFFFFF, false, ww::Rectanglei(-2+viewRect.x/16,-2+viewRect.y/16,viewRect.width/16+4,viewRect.height/16+4));

	sprTiles[leveldata->objects.at(1)->id]->setPosition(px,py);
	vb->pushsprite(sprTiles[leveldata->objects.at(1)->id]);

	vb->update();
	vb->draw();
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