#ifndef SHADERUTILS_HPP
#define SHADERUTILS_HPP

#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>
using namespace std;

extern GLuint programShader;
extern GLint attribute_coord3d, attribute_texcoord, attribute_vweight, attribute_vtangent,
	   attribute_normal, attribute_vbones;
extern GLint uniform_mvp, uniform_texture, uniform_bonemats;

int initResources();
void freeResources();
void printLog(GLuint object);
void print_glm(glm::mat4 m);
GLuint createShader(string filename, GLenum type);
GLuint getAttribute(string name);
GLuint getUniform(string name);
string loadFile(string filename);


#endif
