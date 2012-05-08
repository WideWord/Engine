#include "gfx.h"
using namespace gfx;

#include "GL/glew.h"
#ifdef WINDOWS
	#include "GL/wglew.h"
#else
	#include "GL/glxew.h"
#endif
#include "GL/glfw.h"

Mesh::Mesh(MeshData& dat) {
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	
	glGenBuffers(4, vbo);
	// vbo array: 0 - coor, 1 - nor, 2 - tex, 3 - ind
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, dat.verts * sizeof(float) * 3, dat.coor, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, dat.verts * sizeof(float) * 3, dat.nor, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, dat.verts * sizeof(float) * 2, dat.tex, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, dat.faces * sizeof(unsigned) * 3, dat.ind, GL_STATIC_DRAW);
	
	this->faces = dat.faces;
}

Mesh::~Mesh () {
	glDeleteBuffers(4, vbo);
	glDeleteVertexArrays(1, &vao);
}
