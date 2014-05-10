#include <sstream>
#include "luabinding.hpp"
#include "SpriteBinding.hpp"
#include "../globals.hpp"
using namespace std;

Sprite *l_toSprite(lua_State *l, int pos){
	if(lua_isuserdata(l,pos)){
		Sprite *s = (Sprite*)lua_touserdata(l,pos);
		if(s->magic == SPRITE_MAGIC){
			return s;
		}
	}

	stringstream error;
	error << "Bad argument #" << pos << ", expected sprite got "
		<< luaL_typename(l, pos);
	errorTrace(l, error.str());

	return NULL;
}

int l_createSprite(lua_State *l){
	string tex = l_toString(l, 1);
	Sprite *out = new (lua_newuserdata(l,sizeof(Sprite))) Sprite(tex);
	rendman.sprites.push_back(out);
	luaL_getmetatable(l, "MetaSprite");
	lua_setmetatable(l, -2);
	return 1;
}
int l_spriteSetPos(lua_State *l){
	Sprite *s = l_toSprite(l,1);
	float x = l_toNumber(l,2);
	float y = l_toNumber(l,3);
	float z = l_toNumber(l,4);
	s->position = glm::vec3(x,y,z);
	return 0;
}
int l_spriteSetScale(lua_State *l){
	Sprite *s = l_toSprite(l,1);
	float scale = l_toNumber(l,2);
	s->scale = scale;
	return 0;
}
int l_spriteSetVisible(lua_State *l){
	Sprite *s = l_toSprite(l,1);
	bool show = l_toBool(l, 2);
	s->visible = show;
	return 0;
}
int l_spriteSetFrameSize(lua_State *l){
	Sprite *s = l_toSprite(l,1);
	int xs = l_toNumber(l,2);
	int ys = l_toNumber(l,3);
	s->setFrames(xs,ys);
	return 0;
}
int l_spriteSetAnimate(lua_State *l){
	Sprite *s = l_toSprite(l,1);
	bool anim = l_toBool(l,2);
	s->animate = anim;
	cout << s->animate << endl;
	return 0;
}
int l_spriteDelete(lua_State *l){
	Sprite *s = l_toSprite(l,1);
	for(int i=0;i<rendman.sprites.size();i++){
		if(rendman.sprites[i] == s){
			rendman.sprites.erase(rendman.sprites.begin()+i);
		}
	}
	return 0;
}
