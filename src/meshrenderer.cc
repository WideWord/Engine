#include "gfx.h"
using namespace quby;

MeshRenderer::MeshRenderer (GameObject* obj, SuperMesh* _mesh) : Component(obj, true) {
	if (obj->getComponent<Transform>() == nullptr) throw "Transform not found";
	mesh = _mesh;
	
}

void MeshRenderer::update () {
	Renderer::getSingleton()->vMeshRenderer.push_back(this);
}

