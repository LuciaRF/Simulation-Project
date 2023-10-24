#include "RayTraceGLSL.h"
#define GLAD_ONLY_HEADERS
#include "common.h"
#include "eventmanager.h"

void GLAPIENTRY
MessageCallback( GLenum source,
                 GLenum type,
                 GLuint id,
                 GLenum severity,
                 GLsizei length,
                 const GLchar* message,
                 const void* userParam )
{
  printf(  "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
           ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
            type, severity, message );
}


RayTraceGLSL::RayTraceGLSL()
{
    //init GLSL
    if (!glfwInit())
    {
        printf("ERROR GLFWInit() Fichero : %s  L�nea: %d\n", __FILE__, __LINE__);
        return;
    }
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    GLwindow = glfwCreateWindow(640, 480, "APIS_1", NULL, NULL);
    if (!GLwindow)
    {
        printf("ERROR glfwCreateWindow() Fichero : %s  L�nea: %d\n", __FILE__, __LINE__);
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(GLwindow);
    gladLoadGL(glfwGetProcAddress);

    glEnable              ( GL_DEBUG_OUTPUT );
    glDebugMessageCallback( MessageCallback, 0 );

    //load program
    this->program=new GLSLProgram();
    program->addShader("data/program.compute");
    program->linkProgram();

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "could not initialize sdl2: %s\n", SDL_GetError());
        return ;
    }
    window = SDL_CreateWindow(
        "RayTracer",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        WIDTH, HEIGHT,
        SDL_WINDOW_SHOWN
    );
    if (window == NULL) {
        fprintf(stderr, "could not create window: %s\n", SDL_GetError());
        return ;
    }


    screenSurface = SDL_GetWindowSurface(window);
    std::cout << SDL_GetPixelFormatName(screenSurface->format->format) << "\n";
    std::cout << screenSurface->w << "\n";
    std::cout << screenSurface->h << "\n";
    std::cout << screenSurface->pitch << "\n";
}

void RayTraceGLSL::setupObj(Object3D *obj)
{

}

void RayTraceGLSL::drawObject(Object3D *obj)
{

}

void RayTraceGLSL::dispatchEvent(SDL_Event e)
{
    switch (e.type) {

    case SDL_KEYDOWN:
        eventManager::keyb_State['A'+ e.key.keysym.sym - 'a'] = 1;
        switch (e.key.keysym.sym) {
        case SDLK_ESCAPE:
            exit=true;
            break;

        default:
            break;
        }
        break;
    case SDL_KEYUP:
            eventManager::keyb_State[e.key.keysym.sym - 'a'] = 0;
        break;
    case SDL_WINDOWEVENT:

        switch (e.window.event) {

        case SDL_WINDOWEVENT_CLOSE:
            exit = true;
            break;

        default:
            break;
        }
        break;

    default:
        break;
    }

}

void RayTraceGLSL::drawWorld(World *world)
{

    //Variables para eventos de teclado
    SDL_Event e;
    SDL_PollEvent(&e);
    dispatchEvent(e);
}
