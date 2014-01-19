#ifndef RESOURCEMANAGER_HPP_
#define RESOURCEMANAGER_HPP_

#include "model.hpp"
#include <map>

std::string dataDir = "data/";
std::string modelDir = dataDir+"models/";

class ResourceManager{
private:
	std::map<std::string,Model> models;
public:
	Model *loadModel(std::string name);
};

#endif
