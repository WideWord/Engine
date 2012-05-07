#ifndef GFX_H
#define GFX_H
#include "platform.h"
#include "math3d.h"
#include "scene.h"
#include <vector>

namespace scene {
	class Scene;
}

namespace gfx {
	
	
	class ENGINE_API RenderWindow {
	protected:
		unsigned w, h;
	public:
		RenderWindow(unsigned w, unsigned h, bool fullscreen);
		~RenderWindow();
		void swapBuffers();
	}; 
	
	enum DrawType {
		nolight,
		light,
		shadowmap
	};

	class ENGINE_API Renderer {
	private:
		int _gl_version;
		RenderWindow* wnd;
	public:
		Renderer(RenderWindow* wnd);
		~Renderer();
		const int& gl_version; // readonly
		
		void render (scene::Scene* scn);
	};
	
	
	struct ENGINE_API MeshData {
		unsigned verts;
		float* coor; float* nor; float* tex;
		unsigned faces;
		unsigned* ind;
	};
	
	#define MAT_LIGHT_BIT			0
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
		
	};
	
	class ENGINE_API Mesh {
	protected:
		friend class Renderer;
		unsigned vao, vbo[4];
	public:
		Mesh(MeshData& dat);
		virtual ~Mesh();
		Material* material;
	};
	
	
}

#endif 
