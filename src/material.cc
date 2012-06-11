#include "gfx.h"
using namespace quby;

#include "GL/glew.h"
#ifdef WINDOWS
	#include "GL/wglew.h"
#else
	#include "GL/glxew.h"
#endif
#include "GL/glfw.h"
#include <string>
#include <cstring>
#include <iostream>

Material::Material(const char* uniformDef, const char* fragment) {
    
    const char* vs = "#version 330 core\n\
    uniform mat4 std_m;\
    uniform mat4 std_vp;\
    in vec3 std_position;\
    in vec3 std_normal;\
    in vec2 std_texture_coord;\
    out vec2 _o_uv;\
    out vec3 _o_position;\
    out vec3 _o_normal;\
    void main(void){\
	\
	vec4 vertex = std_m * vec4(std_position, 1);\
	_o_position = vec3(vertex);\
	_o_normal = mat3(std_m) * std_normal;\
	_o_uv = std_texture_coord;\
	gl_Position = std_vp * vertex;\
	}";
	
	
	std::string fs("#version 330 core\n\
	out vec4 _color;\
	in vec2 _o_uv;\
	in vec3 _o_position;\
	in vec3 _o_normal;\
		uniform int std_pointlight_num;\
		struct pointlightstruct { \
		vec3 position;\
		vec3 color;\
		float radius;\
		vec3 attenuation;\
		};\
		uniform pointlightstruct std_pointlight[16];\
		uniform vec3 std_viewposition;");
		
	fs += uniformDef;
	fs += "void main(void){\
	    \
	    vec2 _uv = _o_uv;\
	    vec3 _position = _o_position;\
	    vec3 _normal = _o_normal;";
	
	
	fs += fragment;    
	    
	fs += "vec3 lightColor = vec3(0,0,0);\
	    \
	    for (int _i = 0; _i < std_pointlight_num; ++_i) {\
	        vec3 _lDir = std_pointlight[_i].position - _position;\
	        float _distance = length(_lDir);\
	        float _attenuation = std_pointlight[_i].radius / (std_pointlight[_i].attenuation[0] + std_pointlight[_i].attenuation[1] * _distance + std_pointlight[_i].attenuation[2] * _distance * _distance);\
	        \
	        lightColor += std_pointlight[_i].color * max(0, dot(normalize(_lDir), normalize(_normal))) * _attenuation;\
	    }\
		\
		_color *= vec4(lightColor,1);\
		}";
	
    
    unsigned Vertex, Fragment,Shader, len;
	GLsizei _len;
		
	shader = glCreateProgram();
	Vertex = glCreateShader(GL_VERTEX_SHADER);
	Fragment =  glCreateShader(GL_FRAGMENT_SHADER);
	
	len = strlen(vs);
	glShaderSource(Vertex, 1, (const GLchar**)&vs, (const GLint*)&len);
	glCompileShader(Vertex);
	
	GLint status;
	glGetShaderiv(Vertex, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE)
    {
        char buffer[1024];
        glGetShaderInfoLog(Vertex, 1024, &_len, buffer);
        std::cerr << "Vertex shader compilation failed: " << buffer;
    }
	
	len = fs.length();
	const char* fsource = fs.c_str();
	glShaderSource(Fragment, 1, (const GLchar**)&fsource, (const GLint*)&len);
	glCompileShader(Fragment);
	
	
	glGetShaderiv(Fragment, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE)
    {
        char buffer[1024];
        glGetShaderInfoLog(Fragment, 1024, &_len, buffer);
        std::cerr << "Fragment shader compilation failed: " << buffer;
    }
    
	
	glAttachShader(shader, Vertex);
	glAttachShader(shader, Fragment);
	
	glLinkProgram(shader);
	
	glGetProgramiv(shader, GL_LINK_STATUS, &status);
	if (status != GL_TRUE)
    {
        char buffer[1024];
        glGetProgramInfoLog(shader, 1024, &_len, buffer);
        std::cerr << "Program linking failed: " << buffer;
    }
    
    glUseProgram(shader);
    glValidateProgram(shader);
    
    glGetProgramiv(shader, GL_VALIDATE_STATUS, &status);
	if (status != GL_TRUE)
    {
        char buffer[1024];
        glGetProgramInfoLog(shader, 1024, &_len, buffer);
        std::cerr << "Program validate failed " << buffer;
    }
    
    
}

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
    m->loc = glGetUniformLocation(shader, name);
    m->data.ivalue = param;
    
    checkAddParam(m);
    
}

void Material::addParam(const char* name, float param) {

    MaterialParam* m = new MaterialParam;
    m->type = fvalue;
    m->loc = glGetUniformLocation(shader, name);
    m->data.fvalue = param;
    
    checkAddParam(m);
    
}

void Material::addParam(const char* name, Vector3& param) {

    MaterialParam* m = new MaterialParam;
    m->type = vec3;
    m->loc = glGetUniformLocation(shader, name);
    m->data.vec3 = new Vector3(param);
    
    checkAddParam(m);
    
}

void Material::addParam(const char* name, Texture2d* param) {

    MaterialParam* m = new MaterialParam;
    m->type = tex2d;
    m->loc = glGetUniformLocation(shader, name);
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
