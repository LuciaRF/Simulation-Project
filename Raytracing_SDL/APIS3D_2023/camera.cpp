#include "camera.h"
#include "state.h"
#include "eventmanager.h"
Camera::Camera(glm::vec3 position, glm::vec3 up, glm::vec3 lookAt)
{
	this->entityType = CAMERA;
    this->pos=position;
    this->up=up;
    this->lookAt=lookAt;
    lastMX = lastMY = 0;
    this->direction = lookAt- position;
    direction=glm::normalize(direction); 
    
    rot.y = asin(direction.y);
    rot.x = -acos(direction.x / cos(rot.y));

    clearColor=glm::vec3(1,1,1);
}


void Camera::step(float timestep)
{
    glm::vec3 mov(0.0001f);
    if(eventManager::keyb_State[GLFW_KEY_A])
        {
            pos.x -= 0.001f;
        }

        if (eventManager::keyb_State[GLFW_KEY_D])
        {
            pos.x += 0.001f;
        }

        if (eventManager::keyb_State[GLFW_KEY_W])
        {
            this->pos += direction * 0.001f;
        }
        if (eventManager::keyb_State[GLFW_KEY_S])
        {
            this->pos -= direction * 0.001f;

        }

        if (eventManager::keyb_State[GLFW_KEY_G])
        {
            pos.y -= 0.001f;
        }

        if (eventManager::keyb_State[GLFW_KEY_T])
        {
            pos.y += 0.001f;
        }
		if (eventManager::keyb_State[GLFW_KEY_C])
		{
			rot.y += 0.005f;
		}
		if (eventManager::keyb_State[GLFW_KEY_V])
		{
            rot.y -= 0.005f;
        }
		if (eventManager::keyb_State[GLFW_KEY_N])
		{
			rot.x += 0.005f;
		}

		if (eventManager::keyb_State[GLFW_KEY_B])
		{
            rot.x -= 0.005f;
        }
		if (eventManager::keyb_State[GLFW_KEY_V])
		{
			//lookAt.y += 0.1f;
		}
		if (eventManager::keyb_State[GLFW_KEY_B])
		{
			//lookAt.z -= 0.1f;
		}
		if (eventManager::keyb_State[GLFW_KEY_N])
		{
			//lookAt.z += 0.1f;
		}
	
        
        double mouseX, mouseY;
        double speedMX, speedMY;
        mouseX = mouseY = speedMX = speedMY = 0;

       
        //glfwGetCursorPos(window, &mouseX, &mouseY);
        mouseX = eventManager::mouse_state.xpos;
        mouseY = eventManager::mouse_state.ypos;
       

        float xoffset = mouseX - lastMX;
        float yoffset = lastMY - mouseY; // reversed since y-coordinates range from bottom to top
        lastMX = mouseX;
        lastMY = mouseY;

        const float sensitivity = 0.01f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        rot.x += xoffset;
        rot.y += yoffset;

        if (rot.y > 1.5f)
            rot.y = 1.5f;
        if (rot.y < -1.5f)
            rot.y = -1.50f;

        direction.x = cos(rot.x) * cos(rot.y);
        direction.y = sin(rot.y);
        direction.z = sin(rot.x) * cos(rot.y);
        direction=glm::normalize(direction);
        this->lookAt = this->pos + 3.0f * direction;

}

void Camera::prepare()
{
    State::projectionMatrix = projection;
    State::viewMatrix = glm::lookAt(pos, lookAt, up);
 
}
