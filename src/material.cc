#include "gfx.h"
using namespace quby;

#include "GL/glew.h"
#ifdef WINDOWS
	#include "GL/wglew.h"
#else
	#include "GL/glxew.h"
#endif
#include "GL/glfw.h"

void Material::checkAddParam (MaterialParam* mp) {
    for (std::vector<MaterialParam*>::iterator it = params.begin(); it != params.end(); ++it) {
        MaterialParam* m = *it;
        if (m->loc == mp->loc) {
            *it = mp;
            
            if (m->type == vec3)delete m->data.vec3;
            delete m;
            return;
        }
    }
    params.push_back(mp);
}

void Material::addParam(const char*  name, int param) {

    MaterialParam* m = new MaterialParam;
    m->type = ivalue;
    m->loc = glGetUniformLocation(shader->id, name);
    m->data.ivalue = param;
    
    checkAddParam(m);
    
}

void Material::addParam(const char* name, float param) {

    MaterialParam* m = new MaterialParam;
    m->type = fvalue;
    m->loc = glGetUniformLocation(shader->id, name);
    m->data.fvalue = param;
    
    checkAddParam(m);
    
}

void Material::addParam(const char* name, Vector3& param) {

    MaterialParam* m = new MaterialParam;
    m->type = vec3;
    m->loc = glGetUniformLocation(shader->id, name);
    m->data.vec3 = new Vector3(param);
    
    checkAddParam(m);
    
}

void Material::addParam(const char* name, Texture2d* param) {

    MaterialParam* m = new MaterialParam;
    m->type = tex2d;
    m->loc = glGetUniformLocation(shader->id, name);
    m->data.tex2d = param;
    
    checkAddParam(m);
    
}

Material::~Material () {
    for (std::vector<MaterialParam*>::iterator it = params.begin(); it != params.end(); ++it) {
        MaterialParam* m = *it;
        if (m->type == vec3)delete m->data.vec3;
        delete m;
    }
}
