#include "gfx.h"
using namespace quby;

#include "GL/glew.h"
#ifdef WINDOWS
	#include "GL/wglew.h"
#else
	#include "GL/glxew.h"
#endif
#include "GL/glfw.h"
#include <iostream>
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
	
	std::cout << "OpenGL version " << gl_version << "\n";
	
	glDrawBuffer(GL_BACK);
	
	glViewport(0,0,wnd->w, wnd->h);
	glShadeModel( GL_SMOOTH );
	glClearColor(1.0f, 0.0f, 0.0f, 0.0f); 
	glClearDepth( 1.0f );             
    glEnable( GL_DEPTH_TEST );           
    glDepthFunc( GL_LEQUAL );      
         
	

	
	
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
	
	Camera* cam = nullptr;
	Transform* camTransform = nullptr;
	for (std::vector<Camera*>::iterator it = vCamera.begin(); it != vCamera.end(); ++it) {
		if (dynamic_cast<RenderWindow*>((*it)->target) != nullptr) {
			cam = *it;
			camTransform = cam->gameObject->getComponent<Transform>();
			break;
		}
	}
	if (cam == nullptr)return;
	glViewport(0,0,cam->vpw, cam->vph);
	
	
	float aspectRatio = (float)wnd->w / (float)wnd->h;
	projection.setPerspective(60.0f, aspectRatio, 0.5f, 3000.0f);
	
	for (std::vector<MeshRenderer*>::iterator it = vMeshRenderer.begin(); it != vMeshRenderer.end(); ++it) {
		MeshRenderer* meshRenderer = *it;
		Transform* transform = meshRenderer->gameObject->getComponent<Transform>();
		if (transform == nullptr) throw "Transform not found (Renderer)";
		
			
		Matrix4 model(projection);
		Matrix4 pos;
		Matrix4 rot;
	
		Vector3 v(camTransform->pos);
		Quaternion q(camTransform->rot);
		v.invert();
		q.invert();
		pos.setTranslation(v);
		rot.setRotation(q);
		model *= rot;
		model *= pos;
		
		pos.setTranslation(transform->pos);
		rot.setRotation(transform->rot);
		
		model *= pos;
		model *= rot;
					
				
		
		for(std::vector<Mesh*>::iterator i = meshRenderer->meshes.begin(); i != meshRenderer->meshes.end(); ++i) {
			Mesh* mesh = *i;
			
			unsigned curShader;
			if (mesh->material == nullptr) {
				throw "material == nullptr";
			} else {
				curShader = mesh->material->shader->id;
			}
			
			
			glUseProgram(curShader);
			
			unsigned renderPassLoc = glGetUniformLocation(curShader, "std_render_pass");
			if (renderPassLoc != -1)glUniform1i(renderPassLoc, 0); // 0 - standart, 1 - for shadow map, 2 - for reflection map
			
			
			GLint matrixLocation;
			matrixLocation = glGetUniformLocation(curShader, "std_mvp");
			if (matrixLocation != -1)glUniformMatrix4fv(matrixLocation, 1, GL_TRUE, model.getPtr());
			
			
			glBindVertexArray(mesh->vao);
			glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo[0]);
			
			GLint posLoc = glGetAttribLocation(curShader, "std_position");
			if (posLoc != -1) { 
			    glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		        glEnableVertexAttribArray(posLoc);
		    }
		        
			glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo[2]);
			GLint texLoc = glGetAttribLocation(curShader, "std_texture_coord");
			if (texLoc != -1) {
			    glVertexAttribPointer(texLoc, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
			    glEnableVertexAttribArray(texLoc);
			}
				
				
		   	GLint colorLoc = glGetUniformLocation(curShader, "std_color");
			if (colorLoc != -1)glUniform4f(colorLoc, 
				mesh->material->color.x, 
				mesh->material->color.y,
				mesh->material->color.z, 
				mesh->material->alpha);		    	
	
	
	
		    glBindVertexArray(mesh->vao);
		    
		    glDrawElements(GL_TRIANGLES, mesh->faces * 3, GL_UNSIGNED_INT, NULL);
		}	
		
	}
	
	vMeshRenderer.clear();
	vCamera.clear();
	wnd->swapBuffers();
}

Renderer* Renderer::singleton;

void RenderTarget::_fn () {

}
