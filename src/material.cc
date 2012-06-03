#include "gfx.h"
using namespace quby;

#include "GL/glew.h"
#ifdef WINDOWS
	#include "GL/wglew.h"
#else
	#include "GL/glxew.h"
#endif
#include "GL/glfw.h"


void Material::addParam(const char*  name, int param) {

    MaterialParam* m = new MaterialParam;
    m->type = ivalue;
    m->loc = glGetUniformLocation(shader->id, name);
    m->data.ivalue = param;
    
    params.push_back(m);
    
}

void Material::addParam(const char* name, float param) {

    MaterialParam* m = new MaterialParam;
    m->type = fvalue;
    m->loc = glGetUniformLocation(shader->id, name);
    m->data.fvalue = param;
    
    params.push_back(m);
    
}

void Material::addParam(const char* name, Vector3& param) {

    MaterialParam* m = new MaterialParam;
    m->type = vec3;
    m->loc = glGetUniformLocation(shader->id, name);
    m->data.vec3 = new Vector3(param);
    
    params.push_back(m);
    
}

void Material::addParam(const char* name, Texture2d* param) {

    MaterialParam* m = new MaterialParam;
    m->type = tex2d;
    m->loc = glGetUniformLocation(shader->id, name);
    m->data.tex2d = param;
    
    params.push_back(m);
    
}

Material::~Material () {
    for (std::vector<MaterialParam*>::iterator it = params.begin(); it != params.end(); ++it) {
        MaterialParam* m = *it;
        if (m->type == vec3)delete m->data.vec3;
        delete m;
    }
}
