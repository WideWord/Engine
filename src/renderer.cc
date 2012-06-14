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
#include <string>
#include <sstream>
#include <cmath>



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
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); 
	glClearDepth( 1.0f );             
    glEnable( GL_DEPTH_TEST );           
    glDepthFunc( GL_LEQUAL );
    glEnable(GL_CULL_FACE); 
    glCullFace(GL_BACK);     
         
	

	
	
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
		
			
		Matrix4 model;
		Matrix4 view;
		
		Matrix4 pos;
		Matrix4 rot;
	
		Vector3 v(camTransform->pos);
		Quaternion q(camTransform->rot);
		v.invert();
		q.invert();
		view.setTranslation(v);
		rot.setRotation(q);
		view *= rot;
		
		
		rot.setRotation(transform->rot);
		model.setTranslation(transform->pos);
		model *= rot;
		
		Matrix4 viewProjection(projection);
		viewProjection *= view;
		
		Matrix4 modelView(view);
		modelView *= model;
		
		Matrix4 modelViewProjection(projection);
		modelViewProjection *= view;
		modelViewProjection *= model;
		
		
					
				
		
		for(std::vector<Mesh*>::iterator i = meshRenderer->meshes.begin(); i != meshRenderer->meshes.end(); ++i) {
			Mesh* mesh = *i;
			
			unsigned curShader;
			if (mesh->material == nullptr) {
				throw "material == nullptr";
			} else {
				curShader = mesh->material->shader;
			}
			
			
			glUseProgram(curShader);
			unsigned curTex = 0;
			std::vector<MaterialParam*>::iterator itend = mesh->material->params.end();
			for (std::vector<MaterialParam*>::iterator it = mesh->material->params.begin(); it != itend; ++it) {
                MaterialParam* m = *it;
                
                switch(m->type) {
                    case ivalue:
                        glUniform1i(m->loc, m->data.ivalue);
                        break;
                    case fvalue:
                        glUniform1f(m->loc, m->data.fvalue);
                        break;
                    case vec3:
                        glUniform3f(m->loc, m->data.vec3->x,m->data.vec3->y,m->data.vec3->z);
                        break;
                    case tex2d:
                        glActiveTexture(GL_TEXTURE0 + curTex);
                        glUniform1i(m->loc, curTex);
                        ++curTex;
                        glBindTexture(GL_TEXTURE_2D, m->data.tex2d->id);
                        break;
                
                }
            }
			
			unsigned renderPassLoc = glGetUniformLocation(curShader, "std_render_pass");
			if (renderPassLoc != -1)glUniform1i(renderPassLoc, 0); // 0 - standart, 1 - for shadow map, 2 - for reflection map
			
			unsigned viewposLoc = glGetUniformLocation(curShader, "std_viewposition");
			if (viewposLoc != -1)glUniform3f(viewposLoc, camTransform->pos.x,camTransform->pos.y,camTransform->pos.z);
			
			
			GLint matrixLocation;
			matrixLocation = glGetUniformLocation(curShader, "std_mvp");
			if (matrixLocation != -1)glUniformMatrix4fv(matrixLocation, 1, GL_TRUE, modelViewProjection.getPtr());
			
			GLint mvLocation;
			mvLocation = glGetUniformLocation(curShader, "std_mv");
			if (mvLocation != -1)glUniformMatrix4fv(mvLocation, 1, GL_TRUE, modelView.getPtr());
			
			GLint pLocation;
			pLocation = glGetUniformLocation(curShader, "std_p");
			if (pLocation != -1)glUniformMatrix4fv(pLocation, 1, GL_TRUE, projection.getPtr());
			
			GLint vpLocation;
			vpLocation = glGetUniformLocation(curShader, "std_vp");
			if (vpLocation != -1)glUniformMatrix4fv(vpLocation, 1, GL_TRUE, viewProjection.getPtr());
			
			GLint mLocation;
			mLocation = glGetUniformLocation(curShader, "std_m");
			if (mLocation != -1)glUniformMatrix4fv(mLocation, 1, GL_TRUE, model.getPtr());
			
			
			glBindVertexArray(mesh->vao);
			glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo[0]);
			
			GLint posLoc = glGetAttribLocation(curShader, "std_position");
			if (posLoc != -1) { 
			    glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		        glEnableVertexAttribArray(posLoc);
		    }
		        
		    glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo[1]);
			GLint norLoc = glGetAttribLocation(curShader, "std_normal");
			if (norLoc != -1) {
			    glVertexAttribPointer(norLoc, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
			    glEnableVertexAttribArray(norLoc);
			}
		        
			glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo[2]);
			GLint texLoc = glGetAttribLocation(curShader, "std_texture_coord");
			if (texLoc != -1) {
			    glVertexAttribPointer(texLoc, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
			    glEnableVertexAttribArray(texLoc);
			}
				
				
		  
				
				
		
		    // point lights
		     
		     
		        
		        
		        int i = 0;
		        for (std::vector<PointLight*>::iterator it = vPointLight.begin(); it != vPointLight.end(); ++it) {
		            PointLight* light = *it;
		            Vector3& tr = light->gameObject->getComponent<Transform>()->pos;
		            if (sqrt((tr.x - transform->pos.x)*(tr.x - transform->pos.x) + (tr.y - transform->pos.y)*(tr.y - transform->pos.y) + (tr.z - transform->pos.z)*(tr.z - transform->pos.z)) < light->radius + mesh->radius) {
		                
		                std::stringstream ss1;
		                ss1 << "std_pointlight[" << i << "].position";
		                glUniform3f(glGetUniformLocation(curShader, ss1.str().c_str() ),  tr.x, tr.y, tr.z);
		            
		            
		            
		                std::stringstream ss2;
		                ss2 << "std_pointlight[" << i << "].color";
		                glUniform3f(glGetUniformLocation(curShader, ss2.str().c_str() ),  (*it)->color.x, (*it)->color.y, (*it)->color.z);
		            
		            
		            
		                std::stringstream ss3;
		                ss3 << "std_pointlight[" << i << "].radius";
		                glUniform1f(glGetUniformLocation(curShader, ss3.str().c_str() ),  (*it)->radius);
		                
		                std::stringstream ss4;
		                ss4 << "std_pointlight[" << i << "].attenuation";
		                glUniform3f(glGetUniformLocation(curShader, ss4.str().c_str() ),  (*it)->attenuation.x,  (*it)->attenuation.y,  (*it)->attenuation.z);
		            
		            
		                ++i;
		            }
		        }

		        GLint pointlightnumLoc = glGetUniformLocation(curShader, "std_pointlight_num");
		        if (pointlightnumLoc != -1) glUniform1i(pointlightnumLoc, i);
		     
	
	
	
		    glBindVertexArray(mesh->vao);
		    
		    glDrawElements(GL_TRIANGLES, mesh->faces * 3, GL_UNSIGNED_INT, NULL);
		}	
		
	}
	
	vMeshRenderer.clear();
	vCamera.clear();
	vPointLight.clear();
	wnd->swapBuffers();
}

Renderer* Renderer::singleton;

void RenderTarget::_fn () {

}
