#include "gfx.h"
using namespace quby;

#include "GL/glew.h"
#ifdef WINDOWS
	#include "GL/wglew.h"
#else
	#include "GL/glxew.h"
#endif
#include "GL/glfw.h"


Camera::Camera (GameObject* obj, RenderTarget* target) : Component(obj, true) {
	if (obj->getComponent<Transform>() == nullptr) throw "Transform not found";
	this->target = target;
	RenderWindow* wnd = dynamic_cast<RenderWindow*>(target);
	if (wnd != nullptr) {
		vpw = wnd->w;
		vph = wnd->h;
	}
	
}

void Camera::update () {
	Renderer::getSingleton()->vCamera.push_back(this);
}

Camera::~Camera () {

}
