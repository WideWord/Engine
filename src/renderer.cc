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
#include <iostream>
#include <cstring>

unsigned createShader (const char* VertexSource, const char* FragmentSource) {
	unsigned Vertex, Fragment,Shader, len;
		
	Shader = glCreateProgram();
	Vertex = glCreateShader(GL_VERTEX_SHADER);
	Fragment =  glCreateShader(GL_FRAGMENT_SHADER);
	
	len = strlen(VertexSource);
	glShaderSource(Vertex, 1, (const GLchar**)&VertexSource, (const GLint*)&len);
	glCompileShader(Vertex);
	
	len = strlen(FragmentSource);
	glShaderSource(Fragment, 1, (const GLchar**)&FragmentSource, (const GLint*)&len);
	glCompileShader(Fragment);
	
	glAttachShader(Shader, Vertex);
	glAttachShader(Shader, Fragment);
	
	glLinkProgram(Shader);
	return Shader;
}

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
	shader[0] = createShader (
	"#version 330 core\nuniform mat4 modelViewProjectionMatrix;in vec3 position;void main(void){gl_Position = modelViewProjectionMatrix * vec4(position, 1.0);}",
	"#version 330 core\nout vec4 color;void main(void){color = vec4(0,0,1,1);}");
	
	shader[MAT_DIFF_BIT] = createShader (
	"#version 330 core\nuniform mat4 modelViewProjectionMatrix;in vec3 position;in vec2 texcoord;out vec2 fragTexcoord;void main(void){gl_Position = modelViewProjectionMatrix * vec4(position, 1.0);fragTexcoord = texcoord;}",
	"#version 330 core\nuniform sampler2D diffuseTexture;in vec2 fragTexcoord;out vec4 color;void main(void){color = texture(diffuseTexture, fragTexcoord);}");

	
	
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
	projection.setPerspective(60.0f, aspectRatio, 0.5f, 10.0f);
	
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
			unsigned matType;
			
			if (mesh->material == nullptr) {
				matType = 0;
			} else {
				matType = mesh->material->type;
			}
			
			unsigned curShader = shader[matType];
			glUseProgram(curShader);
			
			
			GLint matrixLocation;
			matrixLocation = glGetUniformLocation(curShader, "modelViewProjectionMatrix");
			if (matrixLocation == -1)throw "error";
			glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, model.getPtr());
			
			glBindVertexArray(mesh->vao);
			glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo[0]);
			
			GLint posLoc = glGetAttribLocation(curShader, "position");
			if (posLoc == -1)throw "error";
			glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
		    glEnableVertexAttribArray(posLoc);
		        
		    if ((matType & MAT_DIFF_BIT) != 0) {
		    	//glBindVertexArray(mesh->vao);
				glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo[2]);
				GLint texLoc = glGetAttribLocation(curShader, "texcoord");
				if (texLoc == -1)throw "error";
				glVertexAttribPointer(texLoc, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
				glEnableVertexAttribArray(texLoc);
				
				
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, mesh->material->diffuse->id);
				
				GLint textureLocation = -1;

				textureLocation = glGetUniformLocation(curShader, "diffuseTexture");

				// укажем, что текстура привязана к текстурному юниту 0
				if (textureLocation != -1)
						glUniform1i(textureLocation , 0);
				std::cout << "textured!\n";
				
		    }
		    std::cout << (unsigned)(matType & MAT_DIFF_BIT);
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
