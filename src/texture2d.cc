#include "gfx.h"
using namespace quby;

#include "GL/glew.h"
#ifdef WINDOWS
	#include "GL/wglew.h"
#else
	#include "GL/glxew.h"
#endif
#include "GL/glfw.h"
#include "stdio.h"
#include "stb_image.h"
#include <iostream>

Texture2d::Texture2d(unsigned w, unsigned h, ImageDataFormat dformat, void* data) {
	
	glEnable(GL_TEXTURE_2D);
	
	GLint maxSize = 0;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxSize);
	
	if ((int)w > maxSize || (int)h > maxSize) {
		std::cerr << "Error, max texture size " << maxSize  << "x" << maxSize << " < " << w << "x" << h << "\n"; 
		return;
	}

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    
    bool usealpha = false;
    if (dformat == BGRA || dformat == RGBA) usealpha = true;
    
    GLint internalFormat = usealpha?GL_RGB8 : GL_RGBA8;
    
    
    GLint format;
    switch (dformat) {
    	case BGR:
    		format = GL_BGR;
    		break;
    	case BGRA:
    		format = GL_BGRA;
    		break;
    	case RGB:
    		format = GL_RGB;
    		break;
    	case RGBA:
    		format = GL_RGBA;
    		break;
    	case floatFormat:
    		internalFormat = GL_RGBA32F;
    		break;
    }
    
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat , w, h, 0, format,
                (dformat==floatFormat)?GL_FLOAT:GL_UNSIGNED_BYTE, (const GLvoid*)(data));
}

Texture2d::~Texture2d () {

}

Texture2d* quby::loadTexture2d(const char* filename) {
	int x, y, comp;
	stbi_uc* uc = stbi_load(filename,     &x, &y, &comp, STBI_default);
	
	ImageDataFormat idformat;
	
	if (comp == STBI_rgb) idformat = RGB;
	if (comp == STBI_rgb_alpha) idformat = RGBA;
	
	Texture2d* res = new Texture2d(x, y, idformat, uc);
	return res;	
}

