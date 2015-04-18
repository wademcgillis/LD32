#include <WadeWork/wadework.h>

bool doinit();
bool dorun();
bool doend();

int main()
{
	ww::gfx::setWindowSize(640,480);
	ww::sys::setInitCallback(doinit);
	ww::sys::setTimerCallback(dorun);
	ww::sys::setDeinitCallback(doend);
	ww::sys::setTimerResolution(30);
	ww::sys::setup(ww::sys::CONFIG_OPENGL1 | ww::sys::CONFIG_DISABLE_OPENAL | ww::sys::CONFIG_DISABLE_OPENGL_DEPTHBUFFER);
}

ww::gfx::Texture *tex;
ww::gfx::Sprite *spr;
ww::gfx::VertexBatch *vb;

bool doinit()
{
	tex = new ww::gfx::Texture();
	tex->load("resources/gfx/fish.png");

	spr = new ww::gfx::Sprite();
	spr->setSize(128,128);
	spr->setPosition(32,32);
	spr->setSubimageUVRect(0,tex->getUVRectFromIntRect(ww::Rectanglei(0,0,128,128)));
	spr->setColor(0xFFFFFFFF);

	vb = new ww::gfx::VertexBatch();
	return true;
}

bool dorun()
{
	glClearColor(.25f,1.f,.5f,1.f);
	glClear(GL_COLOR_BUFFER_BIT);
	ww::gfx::setMatrix(NULL,-1,glm::value_ptr(glm::ortho(0.f,640.f,480.f,0.f,-1.f,1.f)));
	tex->bind();
	vb->clear();
	vb->pushsprite(spr);
	vb->update();
	vb->draw();
	return true;
}

bool doend()
{
	return true;
}