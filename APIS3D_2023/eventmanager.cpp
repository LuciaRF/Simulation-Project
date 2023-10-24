#include "eventmanager.h"
#include <string.h>


mouse_State_t eventManager::mouse_state;
std::map<char,int> eventManager::keyb_State;

eventManager::eventManager()
{

}

void eventManager::onMouseMoveGLFW(GLFWwindow* win, double  xpos, double ypos)
{

    mouse_state.xpos = xpos;
    mouse_state.ypos = ypos;
}

void eventManager::onKeyChangedGLFW(GLFWwindow* win, int key, int scancode, int action, int mods)
{
    keyb_State[key] = action;
}

void eventManager::initEventManagerGLFW(GLFWwindow* win)
{
    memset(&mouse_state, 0, sizeof(mouse_State_t));
    //memset(keyb_State, 0, 1024);
    glfwSetKeyCallback(win, onKeyChangedGLFW);
    glfwSetCursorPosCallback(win, onMouseMoveGLFW);
}
