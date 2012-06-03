#include "gfx.h"
using namespace quby;

MeshRenderer::MeshRenderer (GameObject* obj) : Component(obj, true) {
	if (obj->getComponent<Transform>() == nullptr) throw "Transform not found";
}

void MeshRenderer::update () {
	Renderer::getSingleton()->vMeshRenderer.push_back(this);
}
