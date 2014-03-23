#ifndef RENDERMANAGER_HPP_
#define RENDERMANAGER_HPP_

#include "model.hpp"
#include "FPSCamera.hpp"
#include "../GameObject.hpp"
#include <map>

class RenderManager{
public:
	GLuint framebuffer;
	void render(ShaderProgram *prg, float curTime);
	std::vector<GameObject*> drawList;
	std::vector<FPSCamera*> cameras;

	FPSCamera *currentCam;

	~RenderManager();
};


#endif
