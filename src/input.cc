#include "input.h"
using namespace quby;

#include "GL/glfw.h"
#include <iostream>


InputManager* InputManager::_singleton;
 

bool InputManager::getKey(int key) {
    return glfwGetKey(key) == GLFW_PRESS;
}

bool InputManager::getKey(KeyCode key) {
    int gkey;
    switch (key) {
        case KEY_UP:
            gkey = GLFW_KEY_UP;
            break;
        case KEY_DOWN:
            gkey = GLFW_KEY_DOWN;
            break;
        case KEY_LEFT:
            gkey = GLFW_KEY_LEFT;
            break;
        case KEY_RIGHT:
            gkey = GLFW_KEY_RIGHT;
            break;
            
        case KEY_ESC:
            gkey = GLFW_KEY_ESC;
            break;
    }
    return glfwGetKey(gkey) == GLFW_PRESS;
}

InputManager::InputManager () {
    glfwEnable( GLFW_STICKY_KEYS );
    _singleton = this;

}
