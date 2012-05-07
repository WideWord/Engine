#include "scene.h"

using namespace scene;

GameObject::GameObject (Scene* scn) : scene(scn) {
	scn->gameObjects.push_back(this);
}

GameObject::~GameObject () {

}

void GameObject::update () {
	for(std::list<Component*>::iterator i = components.begin(); i != components.end(); ++i) {
		if ((*i)->need_update) (*i)->update();
	}
}
