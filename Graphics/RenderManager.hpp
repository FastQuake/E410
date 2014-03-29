#ifndef RENDERMANAGER_HPP_
#define RENDERMANAGER_HPP_

#include <map>
#include <SFML/Graphics.hpp>
#include "model.hpp"
#include "FPSCamera.hpp"
#include "../GameObject.hpp"
#include "Light.hpp"

//TODO: Multiple depth textures, lights and shadow shader
class RenderManager{
public:
	GLuint framebuffer;
	//std::vector<GLuint> depthTextures;
	GLuint depthTexture;
	void renderDepth(ShaderProgram *prg, float dt);
	void renderShadow(ShaderProgram *prg);
	void render(ShaderProgram *prg, float dt);
	std::vector<GameObject*> drawList;
	std::vector<FPSCamera*> cameras;
	//std::vector<Light*> lights;
	Light light;

	sf::Image depthimg;

	FPSCamera *currentCam;

	~RenderManager();
};


#endif
