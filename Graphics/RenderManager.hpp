#ifndef RENDERMANAGER_HPP_
#define RENDERMANAGER_HPP_

#include <map>
#include <SFML/Graphics.hpp>
#include "model.hpp"
#include "FPSCamera.hpp"
#include "../GameObject.hpp"

class RenderManager{
public:
	GLuint framebuffer;
	GLuint depthTexture;
	void renderDepth(ShaderProgram *prg, float dt);
	void render(ShaderProgram *prg, float dt);
	std::vector<GameObject*> drawList;
	std::vector<FPSCamera*> cameras;

	sf::Image depthimg;

	FPSCamera *currentCam;

	~RenderManager();
};


#endif
