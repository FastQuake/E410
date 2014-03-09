#include "../globals.hpp"
#include "CameraBinding.hpp"

FPSCamera *l_toCam(lua_State *l, int pos){
	if(lua_isuserdata(l, pos)){
		FPSCamera *out =(FPSCamera*)lua_touserdata(l, pos);  
		if(out->magic == CAMMAGIC)
			return out;
	}
		
	lua_pushstring(l, "Argument in not a Camera");
	lua_error(l);

	return NULL;
}

int l_createCam(lua_State *l){
	rendman.cameras.push_back(new FPSCamera(0,0,0));
	lua_pushlightuserdata(l, rendman.cameras.back());
	return 1;
}
int l_setCam(lua_State *l){
	FPSCamera *cam = l_toCam(l, 1);
	rendman.currentCam = cam;
	return 0;
}
int l_camSetPos(lua_State *l){
	FPSCamera *cam;
	float x,y,z;

	cam = l_toCam(l, 1);
	x = l_toNumber(l, 2);
	y = l_toNumber(l, 3);
	z = l_toNumber(l, 4);

	cam->pos.x = x;
	cam->pos.y = y;
	cam->pos.z = z;

	return 0;
}
int l_camSetRot(lua_State *l){
	FPSCamera *cam;
	float x,y,z;

	cam = l_toCam(l, 1);
	x = l_toNumber(l, 2);
	y = l_toNumber(l, 3);
	z = l_toNumber(l, 4);
	
	cam->angle.x = x;
	cam->angle.y = y;
	cam->angle.z = z;

	return 0;
}
int l_camTurn(lua_State *l){
	FPSCamera *cam;
	float x, y;

	cam = l_toCam(l,1);
	x = l_toNumber(l, 2);
	y = l_toNumber(l, 3);

	cam->turn(glm::vec2(x,y));
	return 0;
}
int l_camMove(lua_State *l){
	FPSCamera *cam;
	float amount;

	cam = l_toCam(l,1);
	amount = l_toNumber(l,2);

	cam->move(amount);
	return 0;
}
int l_camStrafe(lua_State *l){
	FPSCamera *cam;
	float amount;

	cam = l_toCam(l,1);
	amount = l_toNumber(l,2);

	cam->strafe(amount);
	return 0;
}
