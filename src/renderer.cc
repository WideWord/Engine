#include "gfx.h"
using namespace gfx;
using namespace math3d;
using namespace scene;

#include "GL/glew.h"
#ifdef WINDOWS
	#include "GL/wglew.h"
#else
	#include "GL/glxew.h"
#endif
#include "GL/glfw.h"
#include <cstring>

Renderer::Renderer(RenderWindow* wnd) : gl_version(_gl_version) {
	
	glewInit();
	
	_gl_version = -1;
	if(glewIsSupported("GL_VERSION_1_0")) _gl_version = 100;
	if(glewIsSupported("GL_VERSION_1_1")) _gl_version = 110;
	if(glewIsSupported("GL_VERSION_1_2")) _gl_version = 120;
	if(glewIsSupported("GL_VERSION_1_3")) _gl_version = 130;
	if(glewIsSupported("GL_VERSION_1_4")) _gl_version = 140;
	if(glewIsSupported("GL_VERSION_1_5")) _gl_version = 150;
	if(glewIsSupported("GL_VERSION_2_0")) _gl_version = 200;
	if(glewIsSupported("GL_VERSION_2_1")) _gl_version = 210;
	if(glewIsSupported("GL_VERSION_3_0")) _gl_version = 300;
	if(glewIsSupported("GL_VERSION_3_1")) _gl_version = 310;
	if(glewIsSupported("GL_VERSION_3_2")) _gl_version = 320;
	if(glewIsSupported("GL_VERSION_3_3")) _gl_version = 330;
	if(glewIsSupported("GL_VERSION_4_0")) _gl_version = 400;
	if(glewIsSupported("GL_VERSION_4_1")) _gl_version = 410;

	this->wnd = wnd;
	
	singleton = this;
	
	
	glViewport(0,0,wnd->w, wnd->h);
	glShadeModel( GL_SMOOTH );
	glClearColor(1.0f, 0.0f, 0.0f, 0.0f); 
	glClearDepth( 1.0f );             
    glEnable( GL_DEPTH_TEST );           
    glDepthFunc( GL_LEQUAL );      
         
	
	
	// создаём шейдеры
	
	unsigned nolightVertex, nolightFragment, len;
	const char* nolightVertexSource = "#version 330 core\nuniform mat4 modelViewProjectionMatrix;in vec3 position;void main(void){gl_Position = modelViewProjectionMatrix * vec4(position, 1.0);}";
	
	const char* nolightFragmentSource = "#version 330 core\nout vec4 color;void main(void){color = vec4(0,0,1,1);}";
	
	
	nolightShader = glCreateProgram();
	nolightVertex = glCreateShader(GL_VERTEX_SHADER);
	nolightFragment =  glCreateShader(GL_FRAGMENT_SHADER);
	
	len = strlen(nolightVertexSource);
	glShaderSource(nolightVertex, 1, (const GLchar**)&nolightVertexSource, (const GLint*)&len);
	glCompileShader(nolightVertex);
	
	len = strlen(nolightFragmentSource);
	glShaderSource(nolightFragment, 1, (const GLchar**)&nolightFragmentSource, (const GLint*)&len);
	glCompileShader(nolightFragment);
	
	glAttachShader(nolightShader, nolightVertex);
	glAttachShader(nolightShader, nolightFragment);
	
	glLinkProgram(nolightShader);
	
	
}

Renderer* Renderer::getSingleton() {
	return singleton;
}

Renderer::~Renderer() {
}


void Renderer::render () {

	Matrix4 projection;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// рендеринг без света, из точки наблюдения 0, 0, 0,  (камера будет потом)
	glUseProgram(nolightShader);
	
	
	
	float aspectRatio = (float)wnd->w / (float)wnd->h;
	projection.setPerspective(60.0f, aspectRatio, 0.5f, 10.0f);
	
	for (std::vector<MeshRenderer*>::iterator it = vMeshRenderer.begin(); it != vMeshRenderer.end(); ++it) {
		MeshRenderer* meshRenderer = *it;
		Transform* transform = meshRenderer->gameObject->getComponent<Transform>();
		if (transform == nullptr) throw "Transform not found (Renderer)";
		
			
		GLint matrixLocation;
		matrixLocation = glGetUniformLocation(nolightShader, "modelViewProjectionMatrix");
		if (matrixLocation == -1)throw "error";
		glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, projection.getPtr());
		
		for(std::vector<Mesh*>::iterator i = meshRenderer->meshes.begin(); i != meshRenderer->meshes.end(); ++i) {
			Mesh* mesh = *i;
			
			glBindVertexArray(mesh->vao);
			glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo[0]);
				
			GLint posLoc = glGetAttribLocation(nolightShader, "position");
			if (posLoc == -1)throw "error";
			glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		    glEnableVertexAttribArray(posLoc);
		    
		    glBindVertexArray(mesh->vao);
		    
		    glDrawElements(GL_TRIANGLES, mesh->faces * 3, GL_UNSIGNED_INT, NULL);
		}	
		
	}
	
	vMeshRenderer.clear();
	wnd->swapBuffers();
}

Renderer* Renderer::singleton;
