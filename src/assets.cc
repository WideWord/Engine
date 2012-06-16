#include "assets.h"
#include "gfx.h"

#include <iostream>

using namespace quby;
using namespace std;

Texture2d* Assets::getTexture2d (const char* name) {
    string filename(name);
    
    for (vector<pair<string, Texture2d* > >::iterator it = textures.begin(); 
        it != textures.end();  ++it) {
   
        
        if (filename.compare((*it).first) == 0) {
            return (*it).second;
        }
             
    }
    
    Texture2d* result = loadTexture2d(name);
    
    textures.push_back(make_pair(filename, result));   
    
    return result;
}



