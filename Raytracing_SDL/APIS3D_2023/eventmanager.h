#pragma once
#define GLAD_ONLY_HEADERS
#include "common.h"



typedef struct mouse_State_t
{
    double xpos;
    double ypos;
}mouse_State_t;

class eventManager
{
public:

static mouse_State_t mouse_state;
static std::map<char, int> keyb_State;

static void initEventManagerGLFW(GLFWwindow* win);
static void onMouseMoveGLFW(GLFWwindow* win, double  xpos, double ypos);
static void onKeyChangedGLFW(GLFWwindow* win, int key, int scancode, int action, int mods);

private:
    eventManager();

};

