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
	GLuint ubo;
	std::vector<Light> lights;
	void renderDepth(ShaderProgram *prg, float dt, Light light);
	void render(ShaderProgram *prg, float dt);
	std::vector<GameObject*> drawList;
	std::vector<FPSCamera*> cameras;

	sf::Image depthimg;

	FPSCamera *currentCam;

	~RenderManager();
};


#endif
