#include "gfx.h"
using namespace quby;

MeshRenderer::MeshRenderer (GameObject* obj) : Component(obj, true), radius(_radius) {
	if (obj->getComponent<Transform>() == nullptr) throw "Transform not found";
	
}

void MeshRenderer::update () {
	Renderer::getSingleton()->vMeshRenderer.push_back(this);
}

void MeshRenderer::bake () {
    _radius = 0;
    for (std::vector<Mesh*>::iterator it = meshes.begin(); it != meshes.end(); ++it) {
        if (_radius < (*it)->radius)_radius = (*it)->radius;
    }
}
