#ifndef RESOURCEMANAGER_HPP
#define RESOURCEMANAGER_HPP
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <map>
#include "Graphics/model.hpp"
#include "Graphics/Texture.hpp"
#include "GUI/Text.hpp"

extern std::string dataDir;
extern std::string modelDir;
extern std::string fontDir;
extern std::string imgDir;
extern std::string soundDir;
extern std::string defaultFont;
class ResourceManager{
private:
	std::map<std::string,Model> models;
	std::map<std::string,Font> fonts;
	std::map<std::string,Texture> texs;
	std::map<std::string,Mix_Chunk*> sounds;

	std::map<std::string, Model> noGLModels;
public:
	Model *loadModel(std::string name);
	Font *loadFont(std::string name, int size);
	Texture loadTexture(std::string name);
	Mix_Chunk *loadSound(std::string name);

	Model *loadNoGLModel(std::string name);
};

#endif
