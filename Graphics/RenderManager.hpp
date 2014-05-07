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
	GLuint renderbuffer;
	GLuint depthTextures;
	GLuint depthCubemaps;
	GLuint normalTex;
	GLuint ubo;
	void renderDepth(ShaderProgram *prg, float dt, int lightIndex);
	void renderDeferred(ShaderProgram *prg, float dt);
	void updateUBO();
	
	std::vector<GameObject*> drawList;
	std::vector<FPSCamera*> cameras;
	std::vector<Light*> lights;

	FPSCamera *currentCam;
	GameObject skybox;

	GameObject* getId(uint32_t id);
	void render(ShaderProgram *prg, ShaderProgram *skyprg,float curTime);
	void remove(GameObject* obj);
	void removeLight(Light *light);

private:
	void drawScene(ShaderProgram *prg, float dt, bool texture, bool normal);
};


#endif
