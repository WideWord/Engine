#ifndef ASSETS_H
#define ASSETS_H

#include "platform.h"



namespace quby {

    class Texture2d;
    class SuperMesh;
    
    
    class Assets {
    public:
        
        Texture2d* getTexture2d(const char* name);
        SuperMesh* getModel(const char* name);
    
    private:
        std::list<std::pair<std::string, Texture2d*>> textures;
        std::list<std::pair<std::string, SuperMesh*>> superMeshes;   
    };
    
    
    
}

#endif
