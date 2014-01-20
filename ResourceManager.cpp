#include "ResourceManager.hpp"
using namespace std;

string dataDir = "data/";
string modelDir = dataDir+"models/";


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
		loadIQM(modelDir+name,model);
		models[name] = model;
		return &models[name];
	}else{
		return &it->second;
	}
}
