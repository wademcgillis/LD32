#include "LevelData.h"

std::string readExtraString(Extra *extra, std::string name)
{
	unsigned int s = extra->extradata.size();
	for(unsigned int i=0;i<s;i++)
	{
		if (extra->extradata.at(i).name == name)
		{
			if (extra->extradata.at(i).istext)
				return extra->extradata.at(i).text;
			else
				return "";
		}
	}
	return "";
}
int readExtraNumber(Extra *extra, std::string name)
{
	unsigned int s = extra->extradata.size();
	for(unsigned int i=0;i<s;i++)
	{
		if (extra->extradata.at(i).name == name)
		{
			if (!extra->extradata.at(i).istext)
				return extra->extradata.at(i).number;
			else
				return 0;
		}
	}
	return 0;
}

void writeExtraString(Extra *extra, std::string name, std::string text)
{
	unsigned int s = extra->extradata.size();
	for(unsigned int i=0;i<s;i++)
	{
		if (extra->extradata.at(i).name == name)
		{
			if (extra->extradata.at(i).istext)
				extra->extradata.at(i).text = text;
			else
			{
				extra->extradata.at(i).istext = true;
				extra->extradata.at(i).number = 0;
				extra->extradata.at(i).text = text;
				return;
			}
		}
	}
	ExtraData dat;
	dat.istext = true;
	dat.name = name;
	dat.text = text;
	dat.number = 0;
	extra->extradata.push_back(dat);
}
void writeExtraNumber(Extra *extra, std::string name, int number)
{
	unsigned int s = extra->extradata.size();
	for(unsigned int i=0;i<s;i++)
	{
		if (extra->extradata.at(i).name == name)
		{
			if (!extra->extradata.at(i).istext)
				extra->extradata.at(i).number = number;
			else
			{
				extra->extradata.at(i).istext = true;
				extra->extradata.at(i).number = number;
				extra->extradata.at(i).text = "";
				return;
			}
		}
	}
	ExtraData dat;
	dat.istext = false;
	dat.name = name;
	dat.text = "";
	dat.number = number;
	extra->extradata.push_back(dat);
}











void LevelData::reset(unsigned short _width, unsigned short _height)
{
	width = _width;
	height = _height;
	background = new Tile[width*height];
	foreground = new Tile[width*height];
	for(int i=0;i<width*height;i++)
	{
		background[i].id = TILE_NONE;
		background[i].extra = NULL;
		foreground[i].id = TILE_NONE;
		foreground[i].extra = NULL;
	}
	objects.clear();
	views.clear();
}

Tile *LevelData::getForegroundTile(unsigned short x, unsigned short y)
{
	return &foreground[width * y + x];
}
Tile *LevelData::getBackgroundTile(unsigned short x, unsigned short y)
{
	return &background[width * y + x];
}
bool LevelData::pointHitsView(unsigned short x, unsigned short y)
{
	ww::Rectanglei pt(x,y,1,1);
	return rectHitsView(pt);
}
bool LevelData::rectHitsView(ww::Rectanglei rect)
{
	for(unsigned int i=0;i<views.size();i++)
	{
		if (views.at(i).intersects(rect))
			return true;
	}
	return false;
}

bool LevelData::load(std::string path)
{
	//system("pause");
	printf("===== %s =====\n",path.c_str());
	FILE *f = fopen(path.c_str(),"rb");
	if (f == NULL)
		return false;
	char formatID[5] = {0,0,0,0,0};
	fread(formatID,4,1,f);
	if (strcmp(formatID,"poop") == 0)
	{
		// read size
		fread(&width,sizeof(unsigned short),1,f);
		fread(&height,sizeof(unsigned short),1,f);
		printf("Size: %i, %i\n",width,height);
		if (fgetc(f) != 123)
		{
			printf("Failed to read size.\n");
			fclose(f);
		}
		// views
		unsigned short viewCount = 0;
		fread(&viewCount,sizeof(unsigned short),1,f);
		printf("View count: %i\n",viewCount);
		for(int i=0;i<viewCount;i++)
		{
			ww::Rectanglei rect;
			fread(&rect.x,sizeof(int),1,f);
			fread(&rect.y,sizeof(int),1,f);
			fread(&rect.width,sizeof(int),1,f);
			fread(&rect.height,sizeof(int),1,f);
			views.push_back(rect);
			printf("\t View #%i: %i, %i @ %i x %i\n",i,rect.x,rect.y,rect.width,rect.height);
		}
		if (fgetc(f) != 123)
		{
			printf("Failed to read views.\n");
			fclose(f);
		}
		//
		background = new Tile[width*height];
		foreground = new Tile[width*height];
		// read background tiles
		Tile *tile;
		for(unsigned int i=0;i<width*height;i++)
		{
			tile = &background[i];
			fread(&tile->id,sizeof(unsigned short),1,f);
			//printf("\tBGTile %i, %i: %i\n",i%width,i/width,tile.id);
			if (fgetc(f) == 0)
				tile->extra = NULL;
			else
			{
				tile->extra = new Extra();
			}
			if (tile->id > 15)
			{
				printf("%i, %i : %i\n",i%width,i/width,tile->id);
				system("pause");
			}
			//if (i % width == width-1)
			//	printf("\n");
			
		}
		if (fgetc(f) != 123)
		{
			printf("Failed to read background tiles.\n");
			fclose(f);
		}
		// read foreground tiles
		for(unsigned int i=0;i<width*height;i++)
		{
			tile = &foreground[i];
			fread(&tile->id,sizeof(unsigned short),1,f);
			//printf("\tFGTile %i, %i: %i\n",i%width,i/width,tile.id);
			if (fgetc(f) == 0)
				tile->extra = NULL;
			else
			{
				tile->extra = new Extra();
			}
			if (tile->id > 15)
			{
				printf("%i, %i : %i\n",i%width,i/width,tile->id);
				system("pause");
			}
			//printf("%i,",tile.id);
			//if (i % width == width-1)
			//	printf("\n");
		}
		if (fgetc(f) != 123)
		{
			printf("Failed to read foreground tiles.\n");
			fclose(f);
		}
		// read objects
		unsigned short objectCount;
		fread(&objectCount,sizeof(unsigned short),1,f);
		for(unsigned short i=0;i<objectCount;i++)
		{
			Object *object = new Object();
			fread(&object->id,sizeof(unsigned short),1,f);
			fread(&object->x,sizeof(unsigned short),1,f);
			fread(&object->y,sizeof(unsigned short),1,f);
			printf("\tObject #%i: id %i. %i, %i\n",i,object->id,object->x,object->y);
			if (fgetc(f) == 0)
				object->extra = NULL;
			else
			{
				object->extra = new Extra();
			}
			objects.push_back(object);
		}
		if (fgetc(f) != 123)
		{
			printf("Failed to read objects.\n");
			fclose(f);
		}
	}
	return true;
}
bool LevelData::save(std::string path)
{
	FILE *f = fopen(path.c_str(),"wb");
	if (f == NULL)		// why would
		return false;	// this happen
	fwrite("poop",4,1,f); // DATA FORMAT ID
	// write size
	fwrite(&width,sizeof(unsigned short),1,f);
	fwrite(&height,sizeof(unsigned short),1,f);
	fputc(123,f);
	// views
	unsigned short viewCount = views.size();
	fwrite(&viewCount,sizeof(unsigned short),1,f);
	for(int i=0;i<viewCount;i++)
	{
		ww::Rectanglei rect = views.at(i);
		fwrite(&rect.x,sizeof(int),1,f);
		fwrite(&rect.y,sizeof(int),1,f);
		fwrite(&rect.width,sizeof(int),1,f);
		fwrite(&rect.height,sizeof(int),1,f);
	}
	fputc(123,f);
	// read tiles
	Tile tile;
	for(unsigned int i=0;i<width*height;i++)
	{
		tile = background[i];
		fwrite(&tile.id,sizeof(unsigned short),1,f);
		if (tile.extra == NULL)
			fputc(0,f);
		else
		{
			fputc(1,f);
		}
	}
	fputc(123,f);
	for(unsigned int i=0;i<width*height;i++)
	{
		tile = foreground[i];
		fwrite(&tile.id,sizeof(unsigned short),1,f);
		if (tile.extra == NULL)
			fputc(0,f);
		else
		{
			fputc(1,f);
		}
	}
	fputc(123,f);
	// WRITE OBJECTS
	unsigned short objectCount = objects.size();
	fwrite(&objectCount,sizeof(unsigned short),1,f);
	Object *object = NULL;
	for(unsigned short i=0;i<objectCount;i++)
	{
		object = objects.at(i);
		fwrite(&object->id,sizeof(unsigned short),1,f);
		fwrite(&object->x,sizeof(unsigned short),1,f);
		fwrite(&object->y,sizeof(unsigned short),1,f);
		if (object->extra == NULL)
			fputc(0,f);
		else
		{

		}
	}
	fputc(123,f);
	fclose(f);
	return true;
}
