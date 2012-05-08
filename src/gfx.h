#ifndef GFX_H
#define GFX_H
#include "platform.h"
#include "math3d.h"
#include "scene.h"
#include <vector>


namespace gfx {
	
	class ENGINE_API RenderTarget {
	protected:
		virtual void _fn();
	};
	
	class Renderer;
	class ENGINE_API RenderWindow : public RenderTarget {
	protected:
		friend class Renderer;
		friend class Camera;
		unsigned w, h;
	public:
		RenderWindow(unsigned w, unsigned h, bool fullscreen);
		~RenderWindow();
		void swapBuffers();
	
	}; 

	class MeshRenderer;
	class Camera;
	class ENGINE_API Renderer {
	private:
		int _gl_version;
		RenderWindow* wnd;
		static Renderer* singleton;
		
		unsigned shader[128];
	public:
		Renderer(RenderWindow* wnd);
		~Renderer();
		const int& gl_version; // readonly
		
		void render ();
		
		
		static Renderer* getSingleton();
	protected:
		friend class MeshRenderer;
		friend class Camera;
		std::vector<MeshRenderer*> vMeshRenderer;
		std::vector<Camera*> vCamera;
	};
	
	enum ImageDataFormat {
		floatFormat,
		BGR,
		BGRA,
		RGB,
		RGBA		
	};
	
	class ENGINE_API Texture2d : public RenderTarget {
	public:
		Texture2d(unsigned w, unsigned h, ImageDataFormat format, void* data);
		~Texture2d();
	protected:
		unsigned id;
		friend class Renderer;
	};
	Texture2d* loadTexture2d(const char* filename);
	
	struct ENGINE_API MeshData {
		unsigned verts;
		float* coor; float* nor; float* tex;
		unsigned faces;
		unsigned* ind;
	};
	
	#define MAT_LIGHT_BIT			1
	#define MAT_DIFF_BIT			2
	#define MAT_BUMP_BIT			4
	#define MAT_SPEC_BIT			8
	#define MAT_FLAT_MIRROR_BIT		16
	#define MAT_CUBEMAP_MIRROR_BIT	32

	
	class ENGINE_API Material {
	public:
		unsigned type;
		math3d::Vector3 color;
		float alpha;
		
		Texture2d* diffuse;
		
	};
	
	class ENGINE_API Mesh {
	protected:
		friend class Renderer;
		unsigned vao, vbo[4];
		unsigned faces;
	public:
		Mesh(MeshData& dat);
		virtual ~Mesh();
		Material* material;
	};
	
	class ENGINE_API MeshRenderer : public scene::Component {
	public:
		MeshRenderer(scene::GameObject* go);
		//~MeshRenderer();
		std::vector<Mesh*> meshes;
		void update();
	};
	
	class ENGINE_API Camera : public scene::Component {
	public:
		Camera(scene::GameObject* obj, RenderTarget* t);
		~Camera();
		void update();
	protected:
		friend class Renderer;
		RenderTarget* target;
		unsigned vpw, vph;
	};
	
	

}

#endif 
