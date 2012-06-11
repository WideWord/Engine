#ifndef INPUT_H
#define INPUT_H
#include "platform.h"

namespace quby {

    enum KeyCode {
        KEY_UP,
        KEY_DOWN,
        KEY_LEFT,
        KEY_RIGHT,
        KEY_ESC
    
    };
    
    class InputManager {
    public:
        InputManager();
        bool getKey(int key);
        bool getKey(KeyCode key);
        inline static InputManager* getSingleton() {return _singleton;}
    private:
        static InputManager* _singleton;
    };   
    
}

#endif
