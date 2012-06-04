#include "input.h"
using namespace quby;

#include "GL/glfw.h"
#include <iostream>


InputManager* InputManager::_singleton;
 

bool InputManager::getKey(int key) {
    return glfwGetKey(key) == GLFW_PRESS;
}

InputManager::InputManager () {
    glfwEnable( GLFW_STICKY_KEYS );
    _singleton = this;

}
