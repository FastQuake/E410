#ifndef GRAPHICSUTILS_HPP
#define GRAPHICSUTILS_HPP

#include <GL/glew.h>
#include <iostream>
#include <vector>

void printLog(int object);
std::string fileToStr(std::string filename);
bool checkGLError();

class ShaderProgram{
	private:
		GLuint programID;
		GLuint vs, fs;
		std::vector<GLint> attributes;
		std::vector<GLint> uniforms;	

		int compileShader(std::string data, GLenum type);

	public:
		bool good;

		ShaderProgram(std::string vertex, std::string fragment);
		~ShaderProgram();

		GLuint getID();

		GLint getAttribute(int pos);
		GLint getAttribute(std::string name);
		GLint setAttribute(std::string name);

		GLint getUniform(int pos);
		GLint getUniform(std::string name);
		GLint setUniform(std::string name);
};

#endif
