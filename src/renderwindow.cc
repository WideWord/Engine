#include "gfx.h"
using namespace gfx;

#include "GL/glew.h"
#ifdef WINDOWS
	#include "GL/wglew.h"
#else
	#include "GL/glxew.h"
#endif
#include "GL/glfw.h"

RenderWindow::RenderWindow (unsigned w, unsigned h, bool full) {
	glfwInit();
	glfwOpenWindow( w, h, 8,8,8,8, 16, 8, full?GLFW_FULLSCREEN:GLFW_WINDOW);
	
	this->w = w;
	this->h = h;
}

RenderWindow::~RenderWindow () {
	glfwTerminate();
}

void RenderWindow::swapBuffers() {
	glfwSwapBuffers();
}
