#ifndef RENDERMANAGER_HPP_
#define RENDERMANAGER_HPP_

#include <map>
#include <SFML/Graphics.hpp>
#include "model.hpp"
#include "FPSCamera.hpp"
#include "../GameObject.hpp"
#include "Light.hpp"

class RenderManager{
public:
	GLuint framebuffer;
	GLuint depthTextures;
	GLuint depthCubemaps;
	GLuint ubo;
	void renderDepth(ShaderProgram *prg, float dt, int lightIndex);
	void render(ShaderProgram *prg, float dt);
	std::vector<GameObject*> drawList;
	std::vector<FPSCamera*> cameras;
	std::vector<Light*> lights;

	GameObject* getId(uint32_t id);
	void remove(GameObject* obj);
	void removeLight(Light *light);

	FPSCamera *currentCam;
};


#endif
