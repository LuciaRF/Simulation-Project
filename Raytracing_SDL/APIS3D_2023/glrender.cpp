#include "glrender.h"
#include "state.h"
#include "eventmanager.h"
GLRender::GLRender()
{
    

    if (!glfwInit())
    {
        printf("ERROR GLFWInit() Fichero : %s  L�nea: %d\n", __FILE__, __LINE__);
        exit(0);
    }


    window = glfwCreateWindow(640, 480, "APIS_1", NULL, NULL);
    if (!window)
    {
        printf("ERROR glfwCreateWindow() Fichero : %s  L�nea: %d\n", __FILE__, __LINE__);
        glfwTerminate();
        exit(0);
    }
    glfwMakeContextCurrent(window);

    //    glfwSetKeyCallback(window, keyCallback);
    gladLoadGL(glfwGetProcAddress);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);

    eventManager::initEventManagerGLFW(window);
}


//Método que realiza las operaciones necesarias para cargar la malla 3d del objeto en
//memoria de GPU. En concreto, comprobará si no se ha cargado anteriormente sus mallas
void 	GLRender::setupObj(Object3D* obj)
{
    //por cada uno de los mesh
    for (int i = 0; i < obj->getMeshCount(); i++)
    {
        Mesh3D* mesh=obj->getMesh(i);
        if (vMeshIds.find(mesh->getMeshID()) != vMeshIds.end())
            return; //ya está cargado
            //continue;

        glMeshAttribs_t bo;
        //reservar array buffers
        //	vertex buffer
        //  index buffer
        glGenVertexArrays(1, &bo.bufferID);
        glGenBuffers(1, &bo.vertexArrayID);

        glBindVertexArray(bo.bufferID);
        glBindBuffer(GL_ARRAY_BUFFER, bo.vertexArrayID);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_t) * mesh->getVertList()->size(),
            mesh->getVertList()->data(), GL_STATIC_DRAW);

        //copiar indices
        glGenBuffers(1, &bo.indexArrayID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bo.indexArrayID);
        //Ojo!!
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * mesh->getTriangleIdxList()->size(),
            mesh->getTriangleIdxList()->data(), GL_STATIC_DRAW);

        vMeshIds[mesh->getMeshID()] = bo;
    }
}
//Método para dibujar por pantalla un objeto 3D. En este caso, accederá a los atributos
//necesarios del objeto y a su shader para poder dibujarlos en pantalla
void	GLRender::drawObject(Object3D* obj){
    //por cada uno de los mesh
   
    for (int i = 0; i < obj->getMeshCount(); i++)
    {
        Material* material= obj->getMaterial(i);
		State::activeShader = material->getShader();
        State::activeShader->use();
        Mesh3D* mesh=obj->getMesh(i);
        glMeshAttribs_t bo = this->vMeshIds[mesh->getMeshID()];
        //setup buffer object
        glBindVertexArray(bo.bufferID);
        glBindBuffer(GL_ARRAY_BUFFER, bo.vertexArrayID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bo.indexArrayID);
        //setup parameters
            //setup vpos

        obj->computeModelMtx();	
		
		
        material->prepare(obj->getModelMtx(),State::viewMatrix,State::projectionMatrix);
        //obj->updateAABB(obj->getModelMtx() * State::viewMatrix,i);
		if (material->getDepthWrite())
		{
			glEnable(GL_DEPTH_TEST);			
		}
		else
		{
			glDisable(GL_DEPTH_TEST);
		}

        glDrawElements(GL_TRIANGLES, (int)mesh->getTriangleIdxList()->size(), GL_UNSIGNED_INT, nullptr);

    }
}


void	GLRender::drawWorld(World* world){


	State::activeCamera = world->getCamera(world->getActiveCamera());
	State::ambient = world->getAmbient();
	State::activeCamera->prepare();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glm::vec3 clearColor= State::activeCamera->getClearColor();
    glClearColor(clearColor.r, clearColor.g, clearColor.b, 0xFF);

    int nobjects=(int)world->getNumObjects();
	int nemissors = (int)world->getNumEmisors();


	for (int i = 0; i < nobjects; i++)
	{
		drawObject(world->getObject(i));
	}


	for (int i = 0; i < nemissors; i++)
	{
		std::list<Particle*>* partList= world->getEmisor(i)->getParticleList();
		std::list<Particle*>::iterator j;
		for (j = partList->begin(); j != partList->end(); j++)
		{
			setupObj(*j);
			drawObject(*j);
		}
	}

    glfwSwapBuffers(window);
    glfwPollEvents();
	
}
