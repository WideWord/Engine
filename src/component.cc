#include "scene.h"

using namespace scene;


Component::Component (GameObject* go, bool need_update) : gameObject(go) {
	this->need_update = need_update;
	go->components.push_back(this);	
}

Component::~Component () {
	
	for(std::list<Component*>::iterator i = gameObject->components.begin(); i != gameObject->components.end(); ++i) {
		if (*i == this) {
			gameObject->components.erase(i);
			return;
		}
	}
}

void Component::update() {

}
