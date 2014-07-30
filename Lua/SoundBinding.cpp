#include "SoundBinding.hpp"
#include "luabinding.hpp"
#include "../globals.hpp"
using namespace std;

int l_playSound(lua_State *l){
	string name = l_toString(l,1);
	soundman->sounds.push_back(Sound());
	soundman->sounds.back().setSound(name);
	soundman->sounds.back().play();
	return 0;
}
