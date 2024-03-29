#ifndef GFX_H
#define GFX_H
#include "platform.h"
#include "math3d.h"
#include "scene.h"


namespace quby {
	
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
	class PointLight;
	class Material;
	class ENGINE_API Renderer {
	private:
		int _gl_version;
		RenderWindow* wnd;
		static Renderer* singleton;
	public:
		Renderer(RenderWindow* wnd);
		~Renderer();
		const int& gl_version; // readonly
		
		void render ();
		
		
		static Renderer* getSingleton();
	protected:
		friend class MeshRenderer;
		friend class Camera;
		friend class PointLight;
		std::vector<MeshRenderer*> vMeshRenderer;
		std::vector<Camera*> vCamera;
		std::vector<PointLight*> vPointLight;
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
	Texture2d* ENGINE_API loadTexture2d(const char* filename);
	
	struct ENGINE_API MeshData {
		unsigned verts;
		float* coor; float* nor; float* tex;
		unsigned faces;
		unsigned* ind;
	};
	
	

	
	
	enum MaterialParamType {
	    fvalue,
	    ivalue,
	    vec3,
	    tex2d
	};
	
	struct ENGINE_API MaterialParam {
	    unsigned loc;
	    MaterialParamType type;
	    union {
	        Vector3* vec3;
	        Texture2d* tex2d;
	        float fvalue;
	        int ivalue;
	    } data;
	};

	class ENGINE_API Material {
	public:
	    Material(const char* uniformDef, const char* fragment);
	    
		void addParam(const char* name, int param);
		void addParam(const char* name, float param);
		void addParam(const char* name, Texture2d* param);
		void addParam(const char* name, Vector3& param);
		
		void setup();
		
		~Material();
				
	protected:
	    friend class Renderer;
		std::vector<MaterialParam*> params;
    private:
        void checkAddParam(MaterialParam* mp);
        unsigned shader;
	};
	class Mesh;
	
	class ENGINE_API SuperMesh {
    public:
        SuperMesh();
	    std::vector<Mesh*> meshes;
	    void bake();
	    const float& radius;
	private:
	    float _radius;
	};
	
	class ENGINE_API Mesh {
    private:
        float _radius;
	protected:
		friend class Renderer;
		unsigned vao, vbo[4];
		unsigned faces;
		
	public:
	    const float& radius;
		Mesh(MeshData& dat);
		virtual ~Mesh();
		Material* material;
	};
	
	
	class ENGINE_API MeshRenderer : public Component {
	public:
		MeshRenderer(GameObject* go, SuperMesh* _mesh);
		//~MeshRenderer();
		SuperMesh* mesh;
		void update();
	    
	
	};
	SuperMesh* ENGINE_API loadModel(const char* filename,Material** mats, unsigned mats_size);
	
	class ENGINE_API Camera : public Component {
	public:
		Camera(GameObject* obj, RenderTarget* t);
		~Camera();
		void update();
	protected:
		friend class Renderer;
		RenderTarget* target;
		unsigned vpw, vph;
	};
	
	
	class ENGINE_API PointLight : public Component {
	public:
	    PointLight(GameObject* go);
	    void update();
	    
	    float radius;
	    Vector3 color;
	    Vector3 attenuation;
	    
	};
	

}

#endif 
