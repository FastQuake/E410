#include "ResourceManager.hpp"
using namespace std;

string dataDir = "data/";
string modelDir = dataDir+"models/";
string fontDir = dataDir+"fonts/";
string imgDir = dataDir+"textures/";

string defaultFont = "DejaVuSansMono.ttf";


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

GLuint ResourceManager::loadTexture(std::string name){
	map<string, GLuint>::iterator it = texs.find(name);
	if(it == texs.end()){
		GLuint i;
		sf::Image *img = loadImage(name);
		if(img == NULL){
			return -1;
		}
		glGenTextures(1, &i);
		glBindTexture(GL_TEXTURE_2D, i);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D,
				0,
				GL_RGBA,
				img->getSize().x,
				img->getSize().y,
				0,
				GL_RGBA,
				GL_UNSIGNED_BYTE,
				img->getPixelsPtr());
		texs[name] = i;
		cout << "IMAGE X:"<<img->getSize().x << " IMAGE Y:" << img->getSize().y << endl;
		return texs[name];
	} else {
		return it->second;
	}
}

GLuint ResourceManager::loadTexture(std::string name, int *width, int *height){
	map<string, GLuint>::iterator it = texs.find(name);
	if(it == texs.end()){
		GLuint i;
		sf::Image *img = loadImage(name);
		if(img == NULL){
			return -1;
		}
		sf::Vector2u size = img->getSize();
		*width = size.x;
		*height = size.y;
		glGenTextures(1, &i);
		glBindTexture(GL_TEXTURE_2D, i);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D,
				0,
				GL_RGBA,
				size.x,
				size.y,
				0,
				GL_RGBA,
				GL_UNSIGNED_BYTE,
				img->getPixelsPtr());
		texs[name] = i;
		cout << "IMAGE X:"<<img->getSize().x << " IMAGE Y:" << img->getSize().y << endl;
		return texs[name];
	} else {
		return it->second;
	}
}
