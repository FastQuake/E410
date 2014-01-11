#ifndef MODEL_HPP
#define MODEL_HPP

#include <GL/glew.h>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/norm.hpp>
#include "iqm.hpp"
#include "GraphicsUtils.hpp"

struct vertex{
    GLfloat position[3];
    GLfloat normal[3];
    GLfloat tangent[4];
    GLfloat texcoord[2];
    GLubyte blendindex[4];
    GLubyte blendweight[4];
};

class Model {
	private:
		GLuint tris_ebo;
		GLuint verts_vbo;	
		GLuint textureID;
	public:
		std::vector<vertex> verts;
		std::vector<iqmmesh> meshes;
		std::vector<iqmjoint> joints;
		std::vector<GLuint> textureIDS;
		std::vector<glm::mat4> baseframe;
		std::vector<glm::mat4> inversebaseframe;
		std::vector<iqmanim> anims;
		std::vector<iqmpose> poses;
		std::vector<glm::mat4> frames;
		std::vector<glm::mat4> outframe;
		int numFrames;

		void setEBO(GLuint ebo);
		void setVBO(GLuint vbo);
		void setTEXID(GLuint id);

		void draw(ShaderProgram *prg);
		void animate(float currentframe);
};

bool loadIQM(std::string filename, Model &target);
bool loadIQMMesh(std::string filename, iqmheader header, 
		Model &target ,unsigned char *buf);
bool loadIQMAnim(std::string filename, iqmheader header,
		Model &target, unsigned char *buf);



#endif
