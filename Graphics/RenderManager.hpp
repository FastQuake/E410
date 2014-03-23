#ifndef RENDERMANAGER_HPP_
#define RENDERMANAGER_HPP_

#include "model.hpp"
#include "FPSCamera.hpp"
#include "../GameObject.hpp"
#include <map>

class RenderManager{
public:
	FPSCamera *currentCam;
	std::vector<GameObject*> drawList;
	std::vector<FPSCamera*> cameras;

	~RenderManager();

	void render(ShaderProgram *prg, float curTime);
	void remove(GameObject* obj);
};


#endif
