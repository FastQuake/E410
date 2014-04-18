#include <sstream>
#include "../globals.hpp"
#include "CameraBinding.hpp"
using namespace std;

FPSCamera *l_toCam(lua_State *l, int pos){
	if(lua_isuserdata(l, pos)){
		FPSCamera *out =(FPSCamera*)lua_touserdata(l, pos);  
		if(out->magic == CAMMAGIC)
			return out;
	}
		
	stringstream error;
	error << "Bad argument #" << pos << ", expected camera got "
		<< luaL_typename(l, pos);
	errorTrace(l, error.str());

	return NULL;
}

int l_createCam(lua_State *l){
	FPSCamera *out = new (lua_newuserdata(l, sizeof(FPSCamera))) FPSCamera(0,0,0);
	rendman.cameras.push_back(out);
	luaL_getmetatable(l, "MetaCam");
	lua_setmetatable(l, -2);
	return 1;
}
int l_setCam(lua_State *l){
	FPSCamera *cam = l_toCam(l, 1);
	rendman.currentCam = cam;
	return 0;
}
int l_camSetPos(lua_State *l){
	FPSCamera *cam = l_toCam(l, 1);
	float x = l_toNumber(l, 2);
	float y = l_toNumber(l, 3);
	float z = l_toNumber(l, 4);

	cam->pos.x = x;
	cam->pos.y = y;
	cam->pos.z = z;

	return 0;
}
int l_camSetRot(lua_State *l){
	FPSCamera *cam = l_toCam(l, 1);
	float x = l_toNumber(l, 2);
	float y = l_toNumber(l, 3);
	float z = l_toNumber(l, 4);
	
	cam->angle.x = x;
	cam->angle.y = y;
	cam->angle.z = z;

	return 0;
}
int l_camTurn(lua_State *l){
	FPSCamera *cam = l_toCam(l,1);
	float x = l_toNumber(l, 2);
	float y = l_toNumber(l, 3);

	cam->turn(glm::vec2(x,y));
	return 0;
}
int l_camMove(lua_State *l){
	FPSCamera *cam = l_toCam(l,1);
	float amount = l_toNumber(l,2);

	cam->move(amount);
	return 0;
}
int l_camStrafe(lua_State *l){
	FPSCamera *cam = l_toCam(l,1);

	float amount = l_toNumber(l,2);

	cam->strafe(amount);
	return 0;
}
int l_camDelete(lua_State *l){
	FPSCamera *cam = l_toCam(l,1);
	if(cam == rendman.currentCam)
		rendman.currentCam = NULL;
	return 0;
}
