#include "Globals.h"

void draw_rectangle(ww::gfx::VertexBatch *vb, ww::Rectanglei rect, unsigned int color, bool outline, int thickness)
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

void draw_text(ww::gfx::VertexBatch *vb, int x, int y, std::string text, unsigned int scale, unsigned int color)
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

bool draw_bubblebutton(ww::gfx::VertexBatch *vb, int x, int y, ww::gfx::Sprite *spr, unsigned int color)
{
	bool hover = ww::input::mouse::hits(ww::Rectanglei(x,y,48,48));

	highlightybob[1]->setColor(hover?0x80404040:0x80000000);
	highlightybob[1]->setPosition(x,y);
	vb->pushsprite(highlightybob[1]);

	if (spr != NULL)
	{
		ww::vec2dui s = spr->getSize();
		float f = (float)s.x / (float)s.y;
		spr->setColor(0xFFFFFFFF);
		spr->setPosition(x+8+16-16*f,y+8);
		spr->setSize(32 * f,32);
		vb->pushsprite(spr);
		spr->setSize(s.x,s.y);
	}
	
	if (hover)
		highlightybob[0]->setColor(color?color:0xFFFFFFFF);
	else
		highlightybob[0]->setColor(color?color:0xFFA0A0A0);
	highlightybob[0]->setPosition(x,y);
	vb->pushsprite(highlightybob[0]);

	return hover && ww::input::mouse::isButtonPressed(ww::input::mouse::LEFT);
}


ww::gfx::Sprite *MakeTileSprite(ww::gfx::Texture *texture, int x, int y, unsigned int width, unsigned int height)
{
	ww::gfx::Sprite *spr = new ww::gfx::Sprite();
	spr->setSize(width,height);
	spr->setPosition(0,0);
	spr->setSubimageUVRect(0,tex->getUVRectFromIntRect(ww::Rectanglei(x,y,width,height)));
	spr->setColor(0xFFFFFFFF);
	return spr;
}

void drawTiles(ww::gfx::VertexBatch *vb, bool foreground, unsigned int color, bool editor, ww::Rectanglei rect)
{
	static int anim_ticker = 0;
	anim_ticker++;
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
			if (tile->id > 63)
			{
				printf("BAD TILE ID! %i, %i : %i\n",i,j,tile->id);
				continue;
			}
			if (tile->id == TILE_LAVA)
			{
				if (editor)
					sprLava[(anim_ticker/8) % 8]->setPosition(16*(i-rect.x),16*(j-rect.y));
				else
					sprLava[(anim_ticker/8) % 8]->setPosition(16*i,16*j);
				sprTiles[tile->id]->setColor(color);
				vb->pushsprite(sprLava[(anim_ticker/8) % 8]);
			}
			else if (sprTiles[tile->id] != NULL)
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
