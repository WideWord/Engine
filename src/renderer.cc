#include "gfx.h"
using namespace gfx;

#include "GL/glew.h"
#ifdef WINDOWS
	#include "GL/wglew.h"
#else
	#include "GL/glxew.h"
#endif
#include "GL/glfw.h"

Renderer::Renderer(RenderWindow* wnd) : gl_version(_gl_version) {
	
	glewInit();
	
	_gl_version = -1;
	if(glewIsSupported("GL_VERSION_1_0")) _gl_version = 100;
	if(glewIsSupported("GL_VERSION_1_1")) _gl_version = 110;
	if(glewIsSupported("GL_VERSION_1_2")) _gl_version = 120;
	if(glewIsSupported("GL_VERSION_1_3")) _gl_version = 130;
	if(glewIsSupported("GL_VERSION_1_4")) _gl_version = 140;
	if(glewIsSupported("GL_VERSION_1_5")) _gl_version = 150;
	if(glewIsSupported("GL_VERSION_2_0")) _gl_version = 200;
	if(glewIsSupported("GL_VERSION_2_1")) _gl_version = 210;
	if(glewIsSupported("GL_VERSION_3_0")) _gl_version = 300;
	if(glewIsSupported("GL_VERSION_3_1")) _gl_version = 310;
	if(glewIsSupported("GL_VERSION_3_2")) _gl_version = 320;
	if(glewIsSupported("GL_VERSION_3_3")) _gl_version = 330;
	if(glewIsSupported("GL_VERSION_4_0")) _gl_version = 400;
	if(glewIsSupported("GL_VERSION_4_1")) _gl_version = 410;

	this->wnd = wnd;
	
}

Renderer::~Renderer() {
	
}
