#ifndef ASSETS_H
#define ASSETS_H

#include "platform.h"



namespace quby {

    class Texture2d;
    
    class Assets {
    public:
        
        Texture2d* getTexture2d(const char* name);
    
    private:
        std::vector<std::pair<std::string, Texture2d* > > textures;
        
    };
    
}

#endif
