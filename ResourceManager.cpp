#include "ResourceManager.hpp"
using namespace std;

string dataDir = "data/";
string modelDir = dataDir+"models/";
string fontDir = dataDir+"fonts/";
string imgDir = dataDir+"textures/";


/**
 * @param name Name of model, .iqm suffix currently required
 * @return Pointer to the model in the resource manager
 * Returns a pointer to a loaded model from the resource manager,
 * and loads it first if it is not already loaded.
 */
Model *ResourceManager::loadModel(string name){
	map<string,Model>::iterator it = models.find(name);
	//If model is not loaded, load it
	if(it == models.end()){
		Model model;
		if(loadIQM(modelDir+name,model) == false){
			return NULL;
		};
		models[name] = model;
		return &models[name];
	}else{
		return &it->second;
	}
}

sf::Font *ResourceManager::loadFont(string name){
	map<string, sf::Font>::iterator it = fonts.find(name);

	if(it == fonts.end()){
		sf::Font f;
		if(f.loadFromFile(fontDir+name) == false){
			return NULL;
		}
		fonts[name] = f;
		return &fonts[name];
	} else {
		return &it->second;
	}
}

sf::Image *ResourceManager::loadImage(string name){
	map<string, sf::Image>::iterator it = images.find(name);
	if(it == images.end()){
		sf::Image i;
		if(i.loadFromFile(imgDir+name) == false){
			return NULL;
		}
		images[name] = i;
		return &images[name];
	} else {
		return &it->second;
	}
}
