#include "scene.h"

using namespace quby;

void Scene::update () {
    std::list<GameObject*>::iterator itend = gameObjects.end();
    for(std::list<GameObject*>::iterator it = gameObjects.begin(); it != itend; ++it) {
        (*it)->update();
    }
}
