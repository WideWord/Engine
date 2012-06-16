#include "assets.h"
#include "gfx.h"

#include <iostream>


using namespace quby;
using namespace std;

AssetLoadingException::AssetLoadingException(const char* _filename): filename(_filename) {
    AssetLoadingException& t = *this;
    t << "can't load model " << filename;
    
}
AssetLoadingException::~AssetLoadingException () throw() {}


Texture2d* Assets::getTexture2d (const char* name) {
    string filename(name);
    
    
    for (auto it = textures.begin(); 
        it != textures.end();  ++it) {
   
        
        if (filename.compare((*it).first) == 0) {
            return (*it).second;
        }
             
    }
    
    Texture2d* result = loadTexture2d(name);
    
    textures.push_back(make_pair(filename, result));   
    
    return result;
}

SuperMesh* Assets::getModel (const char* name) {
    
    for (auto it = superMeshes.begin(); it != superMeshes.end(); ++it) {
        if (it->first.compare(name) == 0) {
            return it->second;
        }
    }
    
    SuperMesh* result = loadModel(name, nullptr, 0);
    if (result == nullptr) throw AssetLoadingException(name);
    
    superMeshes.push_back(make_pair(string(name), result));
    
    return result;
    
}
