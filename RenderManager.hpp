#ifndef RENDERMANAGER_HPP_
#define RENDERMANAGER_HPP_

#include "model.hpp"
#include "GameObject.hpp"
#include <map>

class RenderManager{
public:
	void render(ShaderProgram *prg);
	std::vector<GameObject*> drawList;

	~RenderManager();
};


#endif
