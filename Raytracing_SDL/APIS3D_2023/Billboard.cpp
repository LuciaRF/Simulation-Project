#include "Billboard.h"
#include "state.h"

Billboard::Billboard() { 	
	this->size = glm::vec3(1, 1,1);
	this->spin = 0;	
}

void Billboard::computeModelMtx() {

		glm::mat4  visor = State::viewMatrix;
		this->modelMtx = glm::transpose(visor);
		this->modelMtx[0][3] = 0;
		this->modelMtx[1][3] = 0;
		this->modelMtx[2][3] = 0;
		this->modelMtx[3] = glm::vec4(this->pos, 1);
		this->modelMtx = glm::scale(this->modelMtx, this->size);
		glm::mat4 rot = glm::rotate(glm::mat4(1.0), this->rot.z, glm::vec3(0, 0, 1));
		this->modelMtx = this->modelMtx* rot;

}