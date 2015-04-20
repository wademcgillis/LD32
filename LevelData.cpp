#include "LevelData.h"

void dumpExtra(Extra *extra)
{
	if (extra == NULL)
		return;
	printf("===Extra data contents===\n");
	unsigned int s = extra->extradata.size();
	for(unsigned int i=0;i<s;i++)
	{
		printf("\tName: %s\n",extra->extradata.at(i).name.c_str());
		if (extra->extradata.at(i).istext)
			printf("\tText: %s\n",extra->extradata.at(i).text.c_str());
		else
			printf("\tNumb: %s\n",extra->extradata.at(i).number);
		printf("\t-----------------\n");
	}
}

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









void LevelData::wipeObjects()
{
	while(objects.size())
	{
		Object *obj = objects.at(0);
		if (obj->extra)
			obj->extra->extradata.clear();
		delete obj->extra;
		obj->extra = NULL;
		delete obj;
		objects.erase(objects.begin());
	}
}
void LevelData::wipe()
{
	for(int i=0;i<width*height;i++)
	{
		background[i].id = TILE_NONE;
		if (background[i].extra)
			background[i].extra->extradata.clear();
		delete background[i].extra;
		background[i].extra = NULL;

		foreground[i].id = TILE_NONE;
		if (foreground[i].extra)
			foreground[i].extra->extradata.clear();
		delete foreground[i].extra;
		foreground[i].extra = NULL;
	}
	delete background;
	delete foreground;
	wipeObjects();
	for(int i=0;i<views.size();i++)
	{
		if (views.at(i).extra != NULL)
			delete views.at(i).extra;
	}
	views.clear();
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
	for(int i=0;i<views.size();i++)
	{
		if (views.at(i).extra != NULL)
			delete views.at(i).extra;
	}
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
		if (views.at(i).view.intersects(rect))
			return true;
	}
	return false;
}







void LevelData::freadExtra(Extra *extra, FILE *f)
{
	// read number of extra data
	unsigned short extraCount = 0;
	fread(&extraCount,sizeof(unsigned short),1,f);
	printf("Reading %i extra data...\n",extraCount);
	// read the data
	ExtraData dat;
	for(int i=0;i<extraCount;i++)
	{
		printf("\t#%i\n",i);
		dat.name = "";
		// read data name
		unsigned short len = 0;
		fread(&len,sizeof(unsigned short),1,f);
		printf("\t\tname: %i: ",len); 
		for(unsigned short j=0;j<len;j++)
			dat.name += fgetc(f);
		printf("%s\n",dat.name.c_str());
		// read whether number or not
		char yesno = fgetc(f);
		if (yesno == 'Y')
			dat.istext = true;
		else if (yesno == 'N')
			dat.istext = false;
		else
			printf("Failed to type of extra data.\n");
		if (dat.istext)
		{
			dat.text = "";
			dat.number = 0;
			// read text
			unsigned short len = 0;
			fread(&len,sizeof(unsigned short),1,f);
			printf("\t\ttext: %i: ",len);
			for(unsigned short j=0;j<len;j++)
				dat.text += fgetc(f);
			printf("%s\n",dat.text.c_str());
		}
		else // read number
		{
			dat.text = "";
			fread(&dat.number,sizeof(int),1,f);
			printf("\t\tnumber: %i\n",dat.number);
		}
		extra->extradata.push_back(dat);
	}
}

void LevelData::fwriteExtra(Extra *extra, FILE *f)
{
	// write number of extra data
	unsigned short extraCount = extra->extradata.size();
	fwrite(&extraCount,sizeof(unsigned short),1,f);
	// write the data
	for(int i=0;i<extra->extradata.size();i++)
	{
		// write data name
		unsigned short len = extra->extradata.at(i).name.length();
		fwrite(&len,sizeof(unsigned short),1,f);
		for(unsigned short j=0;j<len;j++)
			fputc(extra->extradata.at(i).name.at(j),f);
		// write whether number or not
		fputc(extra->extradata.at(i).istext?'Y':'N',f);
		if (extra->extradata.at(i).istext)
		{
			// write text
			unsigned short len = extra->extradata.at(i).text.length();
			fwrite(&len,sizeof(unsigned short),1,f);
			for(unsigned short j=0;j<len;j++)
				fputc(extra->extradata.at(i).text.at(j),f);
		}
		else // write number
			fwrite(&extra->extradata.at(i).number,sizeof(int),1,f);
	}
}








bool LevelData::load(std::string path)
{
	//system("pause");
	wipe();
	printf("===== %s =====\n",path.c_str());
	path = "resources/levels/"+path;
	FILE *f = fopen(path.c_str(),"rb");
	if (f == NULL)
		return false;
	char formatID[5] = {0,0,0,0,0};
	fread(formatID,4,1,f);
	if (strcmp(formatID,"poop") != 0 && strcmp(formatID,"peep") != 0)
		return false;

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
		View v;
		fread(&v.view.x,sizeof(int),1,f);
		fread(&v.view.y,sizeof(int),1,f);
		fread(&v.view.width,sizeof(int),1,f);
		fread(&v.view.height,sizeof(int),1,f);
		if (strcmp(formatID,"peep") == 0)
		{
			if (fgetc(f) == 0)
				v.extra = NULL;
			else
			{
				v.extra = new Extra();
				freadExtra(v.extra,f);
			}
		}
		else
			v.extra = NULL;
		views.push_back(v);
		printf("\t View #%i: %i, %i @ %i x %i\n",i,v.view.x,v.view.y,v.view.width,v.view.height);
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
			freadExtra(tile->extra,f);
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
			freadExtra(tile->extra,f);
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
			freadExtra(object->extra,f);
		}
		objects.push_back(object);
	}
	if (fgetc(f) != 123)
	{
		printf("Failed to read objects.\n");
		fclose(f);
	}
	return true;
}
bool LevelData::save(std::string path)
{
	path = "resources/levels/"+path;
	FILE *f = fopen(path.c_str(),"wb");
	if (f == NULL)		// why would
		return false;	// this happen
	fwrite("peep",4,1,f); // DATA FORMAT ID
	// write size
	fwrite(&width,sizeof(unsigned short),1,f);
	fwrite(&height,sizeof(unsigned short),1,f);
	fputc(123,f);
	// views
	unsigned short viewCount = views.size();
	fwrite(&viewCount,sizeof(unsigned short),1,f);
	for(int i=0;i<viewCount;i++)
	{
		View v = views.at(i);
		fwrite(&v.view.x,sizeof(int),1,f);
		fwrite(&v.view.y,sizeof(int),1,f);
		fwrite(&v.view.width,sizeof(int),1,f);
		fwrite(&v.view.height,sizeof(int),1,f);
		if (v.extra == NULL)
			fputc(0,f);
		else
		{
			fputc(1,f);
			fwriteExtra(v.extra,f);
		}
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
			fwriteExtra(tile.extra,f);
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
			fwriteExtra(tile.extra,f);
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
			fputc(1,f);
			fwriteExtra(object->extra,f);
		}
	}
	fputc(123,f);
	fclose(f);
	return true;
}
