#include "gfx.h"
using namespace quby;

#include "GL/glew.h"
#ifdef WINDOWS
	#include "GL/wglew.h"
#else
	#include "GL/glxew.h"
#endif
#include "GL/glfw.h"
#include <cstring>
#include <iostream>


Shader::Shader (const char* VertexSource, const char* FragmentSource) {
	unsigned Vertex, Fragment,Shader, len;
	GLsizei _len;
		
	id = glCreateProgram();
	Vertex = glCreateShader(GL_VERTEX_SHADER);
	Fragment =  glCreateShader(GL_FRAGMENT_SHADER);
	
	len = strlen(VertexSource);
	glShaderSource(Vertex, 1, (const GLchar**)&VertexSource, (const GLint*)&len);
	glCompileShader(Vertex);
	
	GLint status;
	glGetShaderiv(Vertex, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE)
    {
        char buffer[1024];
        glGetShaderInfoLog(Vertex, 1024, &_len, buffer);
        std::cerr << "Vertex shader compilation failed: " << buffer;
    }
	
	len = strlen(FragmentSource);
	glShaderSource(Fragment, 1, (const GLchar**)&FragmentSource, (const GLint*)&len);
	glCompileShader(Fragment);
	
	
	glGetShaderiv(Fragment, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE)
    {
        char buffer[1024];
        glGetShaderInfoLog(Fragment, 1024, &_len, buffer);
        std::cerr << "Fragment shader compilation failed: " << buffer;
    }
    
	
	glAttachShader(id, Vertex);
	glAttachShader(id, Fragment);
	
	glLinkProgram(id);
	
	glGetProgramiv(id, GL_LINK_STATUS, &status);
	if (status != GL_TRUE)
    {
        char buffer[1024];
        glGetProgramInfoLog(id, 1024, &_len, buffer);
        std::cerr << "Program linking failed: " << buffer;
    }
    
    glUseProgram(id);
    glValidateProgram(id);
    
    glGetProgramiv(id, GL_VALIDATE_STATUS, &status);
	if (status != GL_TRUE)
    {
        char buffer[1024];
        glGetProgramInfoLog(id, 1024, &_len, buffer);
        std::cerr << "Program validate failed " << buffer;
    }
    
}
