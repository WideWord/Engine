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


Shader::Shader (const char* VertexSource, const char* FragmentSource) {
	unsigned Vertex, Fragment,Shader, len;
		
	id = glCreateProgram();
	Vertex = glCreateShader(GL_VERTEX_SHADER);
	Fragment =  glCreateShader(GL_FRAGMENT_SHADER);
	
	len = strlen(VertexSource);
	glShaderSource(Vertex, 1, (const GLchar**)&VertexSource, (const GLint*)&len);
	glCompileShader(Vertex);
	
	len = strlen(FragmentSource);
	glShaderSource(Fragment, 1, (const GLchar**)&FragmentSource, (const GLint*)&len);
	glCompileShader(Fragment);
	
	glAttachShader(id, Vertex);
	glAttachShader(id, Fragment);
	
	glLinkProgram(id);
}
