#ifndef RENDERMANAGER_HPP_
#define RENDERMANAGER_HPP_

#include <map>
#include "model.hpp"
#include "FPSCamera.hpp"
#include "../GameObject.hpp"

class RenderManager{
public:
	FPSCamera *currentCam;
	std::vector<GameObject*> drawList;
	std::vector<FPSCamera*> cameras;

	GameObject* getId(uint32_t id);
	void render(ShaderProgram *prg, float curTime);
	void remove(GameObject* obj);
};


#endif
