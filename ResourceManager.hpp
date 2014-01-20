#ifndef RESOURCEMANAGER_HPP_
#define RESOURCEMANAGER_HPP_

#include "model.hpp"
#include <map>

extern std::string dataDir;
extern std::string modelDir;
class ResourceManager{
private:
	std::map<std::string,Model> models;
public:
	Model *loadModel(std::string name);
};

#endif
