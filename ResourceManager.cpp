#include <sstream>
#include "ResourceManager.hpp"
using namespace std;

string dataDir = "data/";
string modelDir = dataDir+"models/";
string fontDir = dataDir+"fonts/";
string imgDir = dataDir+"textures/";
string soundDir = dataDir+"audio/";

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

Model *ResourceManager::loadNoGLModel(std::string name){
	map<string,Model>::iterator it = noGLModels.find(name);
	//If model is not loaded, load it
	if(it == noGLModels.end()){
		Model model;
		if(noGLLoadIQM(modelDir+name,model) == false){
			return NULL;
		};
		noGLModels[name] = model;
		return &noGLModels[name];
	}else{
		return &it->second;
	}
}

Font *ResourceManager::loadFont(string name, int size){
	stringstream ss;
	ss << name << ":" << size;	
	map<string, Font>::iterator it = fonts.find(ss.str());
	string filename = fontDir + name;
	if(it == fonts.end()){
		TTF_Font *f;
		f = TTF_OpenFont(filename.c_str(),size);
		if(!f){
			cout << TTF_GetError() << endl;
			return NULL;
		}
		Font out;
		out.name = name;
		out.f = f;
		out.size = size;
		fonts[ss.str()] = out;
		return &fonts[ss.str()];
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
sf::SoundBuffer *ResourceManager::loadSound(string name){
	map<string, sf::SoundBuffer>::iterator it = sounds.find(name);
	if(it == sounds.end()){
		sf::SoundBuffer b;
		if(b.loadFromFile(soundDir+name) == false){
			return NULL;
		}
		sounds[name] = b;
		return &sounds[name];
	} else {
		return &it->second;
	}
}
Texture ResourceManager::loadTexture(string name){
	name = imgDir + name;	
	if(name.length() == 0){
		Texture error;
		error.id = -1;
		cerr << "Texture name is empty." << endl;
		return error;
	}
	map<string, Texture>::iterator it = texs.find(name);
	if(it == texs.end()){
		GLuint i;
		SDL_Surface *img = IMG_Load(name.c_str());
		if(!img){
			Texture error;
			error.id = -1;
			return error;
		}

		Texture out;
		out.width = img->w;
		out.height = img->h;	
		//Get texture format
		int tformat = GL_RGBA;
		switch(img->format->BytesPerPixel){
			//If 4 colours or RGBA most likely
			case 4:
				if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
					tformat = GL_BGRA;
				else
					tformat = GL_RGBA;
				break;
			//If 3 colours of RGB most likely
			case 3:
				if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
					tformat = GL_BGR;
				else
					tformat = GL_RGB;
				break;
		}

		glGenTextures(1, &i);
		glBindTexture(GL_TEXTURE_2D, i);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D,
				0,
				tformat,
				out.width,
				out.height,
				0,
				tformat,
				GL_UNSIGNED_BYTE,
				img->pixels);
		cout << "IMAGE X:"<<out.width << " IMAGE Y:" << out.height << endl;
		out.id = i;
		texs[name] = out;
		SDL_FreeSurface(img);
		return texs[name];
	} else {
		return it->second;
	}
}
