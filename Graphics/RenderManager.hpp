#ifndef RENDERMANAGER_HPP_
#define RENDERMANAGER_HPP_

#include <map>
#include <bullet/btBulletDynamicsCommon.h>
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
	GameObject* getBody(const btCollisionObject *body);
	void render(ShaderProgram *prg, ShaderProgram *skyprg,float curTime);
	void remove(GameObject* obj);
	void removeLight(Light *light);

private:
	void drawScene(ShaderProgram *prg, float dt, bool texture, bool normal);
};

class GLDebugDrawer : public btIDebugDraw
{
	int m_debugMode;

public:
	GLDebugDrawer();
	virtual ~GLDebugDrawer();

	virtual void drawLine(const btVector3& from,const btVector3& to,const btVector3&  fromColor, const btVector3& toColor);
	virtual void drawLine(const btVector3& from,const btVector3& to,const btVector3& color);
	virtual void drawSphere (const btVector3& p, btScalar radius, const btVector3& color);
	virtual void drawTriangle(const btVector3& a,const btVector3& b,const btVector3& c,const btVector3& color,btScalar alpha);
	virtual void drawContactPoint(const btVector3& PointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color);
	virtual void reportErrorWarning(const char* warningString);
	virtual void draw3dText(const btVector3& location,const char* textString);
	virtual void setDebugMode(int debugMode);
	virtual int getDebugMode() const;
};

extern ShaderProgram *debugprg;

#endif
