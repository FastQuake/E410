#include <iostream>
#include <sstream>
#include "../globals.hpp"
#include "../Networking/server.hpp"
#include "luabinding.hpp"
#include "GameObjectBinding.hpp"
using namespace std;

GameObject *l_toGO(lua_State *l, int pos){
	if(lua_isuserdata(l,pos)){
		 GameObject *out = (GameObject*)lua_touserdata(l,pos);
		 if(out->magic == GOMAGIC)
			 return out;
	}

	stringstream error;
	error << "Bad argument #" << pos << ", expected gameobject got "
		<< luaL_typename(l, pos);
	errorTrace(l, error.str());

	return NULL;
}

int l_loadIQM(lua_State *l){
	string model = l_toString(l, 1);

	Model *mod = resman.loadModel(model);
	if(mod == NULL){
		lua_pushstring(l,("Could not load model "+model).c_str());
		lua_error(l);
	}

	GameObject *out = new (lua_newuserdata(l, sizeof(GameObject))) GameObject;
	out->setModel(mod);
	rendman.drawList.push_back(out);
	luaL_getmetatable(l, "MetaGO");
	lua_setmetatable(l, -2);

	return 1;
}
int l_serverLoadIQM(lua_State *l){
	string model = l_toString(l, 1);
	string tag = l_toString(l, 2);

	Model *mod = resman.loadNoGLModel(model); 
	if(mod == NULL){
		lua_pushstring(l,("Could not load model "+model).c_str());
		lua_error(l);
	}

	GameObject *out = new (lua_newuserdata(l, sizeof(GameObject))) GameObject;
	out->setModel(mod);
	out->modelName = model;
	out->id = serverID++;
	out->tag = tag;
	out->createConvexRidgidBody();
	serverRendMan.drawList.push_back(out);
	luaL_getmetatable(l, "MetaGO");
	lua_setmetatable(l, -2);

	ENetPeer peer;
	peer.address.host = -1;

	sendCreatePacket(&peer,out);

	return 1;
}

int l_setPos(lua_State *l){
	GameObject *obj = l_toGO(l, 1);

	float x = l_toNumber(l,2);
	float y = l_toNumber(l,3);
	float z = l_toNumber(l,4);

	obj->position.x = x;
	obj->position.y = y;
	obj->position.z = z;
	obj->moved = true;
	return 0;
}

int l_setRot(lua_State *l){
	GameObject *obj = l_toGO(l, 1);

	float x = l_toNumber(l,2);
	float y = l_toNumber(l,3);
	float z = l_toNumber(l,4);

	obj->rotation.x = x;
	obj->rotation.y = y;
	obj->rotation.z = z;
	obj->rotated = true;
	obj->updateLookat();
	return 0;
}

int l_setScale(lua_State *l){
	GameObject *obj = l_toGO(l, 1);

	float x = l_toNumber(l,2);
	float y = l_toNumber(l,3);
	float z = l_toNumber(l,4);

	obj->scale.x = x;
	obj->scale.y = y;
	obj->scale.z = z;
	obj->scaled = true;
	return 0;
}
int l_move(lua_State *l){
	GameObject *obj = l_toGO(l, 1);
	float amount = l_toNumber(l, 2);

	obj->move(amount);
	obj->moved = true;
	return 0;
}
int l_strafe(lua_State *l){
	GameObject *obj = l_toGO(l, 1);
	float amount = l_toNumber(l, 2);

	obj->strafe(amount);
	obj->moved = true;
	return 0;
}
int l_turn(lua_State *l){
	GameObject *obj = l_toGO(l, 1);
	float x = l_toNumber(l, 2);
	float y = l_toNumber(l, 3);

	obj->turn(glm::vec2(x,y));
	obj->rotated = true;
	return 0;
}
int l_setCurAnim(lua_State *l){
	GameObject *obj = l_toGO(l, 1);
	string name = l_toString(l, 2);
	
	obj->aTime = 0;
	obj->currentAnimation = name;
	return 0;
}

int l_setAnim(lua_State *l){
	GameObject *obj = l_toGO(l, 1);
	bool animate = l_toBool(l, 2);

	if(animate && !obj->hasAnimation){
		lua_pushstring(l,"Object contains no animations");
		lua_error(l);
	}
	obj->animate = animate;
	return 0;
}
int l_setTexture(lua_State *l){
	GameObject *obj = l_toGO(l, 1);
	string tex = l_toString(l, 2);
	
	GLuint id = resman.loadTexture(tex);
	if(id == -1){
		cout << "Can't find texture " << tex << endl;
		id = resman.loadTexture("default.png");
	}
	obj->textures.at(0) = id;
	return 0;
}
int l_setVisible(lua_State *l){
	GameObject *obj = l_toGO(l,1);
	bool vis = l_toBool(l,2);

	obj->visible = vis;
	return 0;
}
int l_getPos(lua_State *l){
	GameObject *obj = l_toGO(l,1);

	lua_pushnumber(l,obj->position.x);
	lua_pushnumber(l,obj->position.y);
	lua_pushnumber(l,obj->position.z);
	return 3;
}
int l_getTag(lua_State *l){
	GameObject *obj = l_toGO(l,1);

	lua_pushstring(l,obj->tag.c_str());
	return 1;
}
int l_delete(lua_State *l){
	lua_pushnil(l);
	GameObject *obj = l_toGO(l, 1);

	rendman.remove(obj);
	return 0;
}
int l_serverDelete(lua_State *l){
	lua_pushnil(l);
	GameObject *obj = l_toGO(l, 1);

	serverRendMan.remove(obj);
	return 0;
}
int l_setV(lua_State *l){
	GameObject *obj = l_toGO(l,1);
	float x = l_toNumber(l,2);
	float y = l_toNumber(l,3);
	float z = l_toNumber(l,4);

	if(obj->body != NULL)
		obj->body->setLinearVelocity(btVector3(x,y,-z));

	return 0;
}
int l_setMass(lua_State *l){
	GameObject *obj = l_toGO(l,1);
	float mass = l_toNumber(l,2);
	obj->updateMass(mass);
	return 0;
}
int l_serverSetPos(lua_State *l){
	GameObject *obj = l_toGO(l,1);
	float x = l_toNumber(l,2);
	float y = l_toNumber(l,3);
	float z = l_toNumber(l,4);

	if(obj->motion != NULL){
		obj->motion->setWorldTransform(btTransform(obj->rot,
					btVector3(x,y,z)));
		obj->body->setMotionState(obj->motion);
	}
	obj->position = glm::vec3(x,y,z);
	obj->moved = true;
	return 0;
}
int l_serverSetRot(lua_State *l){
	GameObject *obj = l_toGO(l, 1);
	float x = l_toNumber(l,2);
	float y = l_toNumber(l,3);
	float z = l_toNumber(l,4);

	if(obj->motion != NULL){
		btTransform trans;
		obj->motion->getWorldTransform(trans);
		obj->motion->setWorldTransform(btTransform(btQuaternion(toRad(x),toRad(y),toRad(z)),
					trans.getOrigin()));
		obj->body->setMotionState(obj->motion);
	}
	obj->rotation.x = x;
	obj->rotation.y = y;
	obj->rotation.z = z;
	obj->rotated = true;
	obj->updateLookat();
	return 0;
}
int l_getLookat(lua_State *l){
	GameObject *obj = l_toGO(l,1);
	lua_pushnumber(l, obj->lookat.x);
	lua_pushnumber(l, obj->lookat.y);
	lua_pushnumber(l, obj->lookat.z);
	return 3;
}

int l_serverSetScale(lua_State *l){
	GameObject *obj = l_toGO(l,1);
	float x = l_toNumber(l,2);
	float y = l_toNumber(l,3);
	float z = l_toNumber(l,4);

	if(obj->body != NULL){
		obj->body->getCollisionShape()->setLocalScaling(btVector3(x,y,z));
	}
	obj->scale = glm::vec3(x,y,z);
	obj->scaled = true;
	return 0;
}
int l_setConvexBody(lua_State *l){
	GameObject *obj = l_toGO(l,1);
	obj->createConvexRidgidBody();
	return 0;
}
int l_setTriangleBody(lua_State *l){
	GameObject *obj = l_toGO(l,1);
	obj->createTriangleRidgidBody();
	return 0;
}
int l_setBoxBody(lua_State *l){
	GameObject *obj = l_toGO(l,1);
	obj->createCubeRidgidBody();
	return 0;
}

int l_serverLockAxis(lua_State *l){
	GameObject *obj = l_toGO(l,1);
	float x = l_toNumber(l,2);
	float y = l_toNumber(l,3);
	float z = l_toNumber(l,4);
	if(obj->body != NULL){
		physworld.removeBody(obj->body);
		obj->body->setAngularFactor(btVector3(x,y,z));
		physworld.addBody(obj->body);
	}
	return 0;
}

