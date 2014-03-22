#ifndef RESOURCEMANAGER_HPP_
#define RESOURCEMANAGER_HPP_

#include <SFML/Graphics.hpp>
#include <map>
#include "Graphics/model.hpp"

extern std::string dataDir;
extern std::string modelDir;
extern std::string fontDir;
class ResourceManager{
private:
	std::map<std::string,Model> models;
	std::map<std::string,sf::Font> fonts;
public:
	Model *loadModel(std::string name);
	sf::Font *loadFont(std::string name);
};

#endif
