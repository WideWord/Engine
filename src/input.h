#ifndef INPUT_H
#define INPUT_H
#include "platform.h"

namespace quby {

    
    class InputManager {
    public:
        InputManager();
        bool getKey(int key);
        inline static InputManager* getSingleton() {return _singleton;}
    private:
        static InputManager* _singleton;
    };   
    
}

#endif
