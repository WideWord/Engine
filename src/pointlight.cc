#include "gfx.h"
using namespace quby;

PointLight::PointLight(GameObject* obj) : Component(obj, true) {
    if (obj->getComponent<Transform>() == nullptr) throw "Transform not found";
}

void PointLight::update () {
    Renderer::getSingleton()->vPointLight.push_back(this);
}
