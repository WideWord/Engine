#ifndef SCENE_H
#define SCENE_H
#include "platform.h"
#include "math3d.h"
#include <list>


namespace quby {
	
	class GameObject;
	class ENGINE_API Component {
	public:
		Component (GameObject* obj, bool need_update = false);
		virtual ~Component();
		virtual void update();
		GameObject* gameObject;
	protected:
		friend class GameObject;
		bool need_update;
	};

	class Scene;
	class ENGINE_API GameObject {
	public:
		GameObject(Scene* scn);
		~GameObject();
		template<class T> inline T* getComponent() {
			for(std::list<Component*>::iterator i = components.begin(); i != components.end(); ++i) {
				Component* c = *i;
				T* t = dynamic_cast<T*>(c);
				if (t != nullptr) return t;
			}
			return nullptr;
		}
		void update();
		Scene* scene;
	protected:
		friend class Component;
		std::list<Component*> components;
	};
	
	class ENGINE_API Scene {
	public:
	    void update();
	protected:
		friend class GameObject;
		std::list<GameObject*> gameObjects;
	};
	
	class ENGINE_API Transform : public Component {
	public:
		Transform(GameObject* go);
		Vector3 pos;
		Quaternion rot;
	};
	
	

}

#endif
