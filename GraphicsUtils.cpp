#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "GraphicsUtils.hpp"
using namespace std;

void printLog(int object){
	GLint logLength = 0;
	if(glIsShader(object))
		glGetShaderiv(object, GL_INFO_LOG_LENGTH, &logLength);
	else if(glIsProgram(object))
		glGetProgramiv(object, GL_INFO_LOG_LENGTH, &logLength);
	else{
		cerr << "PringLog: Not a shader or a program" << endl;
		return;
	}

	char *log = new char[logLength];
	if(glIsShader(object))
		glGetShaderInfoLog(object,logLength,NULL,log);
	else if(glIsProgram(object))
		glGetProgramInfoLog(object,logLength,NULL,log);

	cerr << log << endl;
	delete log;
}

string fileToStr(string filename){
	ifstream file;
	stringstream buf;
	const char *cname = filename.c_str();
	file.open(cname);
	if(file.is_open()){
		buf << file.rdbuf();
	} else {
		cerr << "Could not open file: " << filename << endl;
		file.close();
		return "";
	}


	file.close();
	return buf.str();
}

bool checkGLError(){
	GLenum err = glGetError();

	if(err != GL_NO_ERROR){
		switch(err){
			case GL_INVALID_OPERATION:
				cerr << "Invalid OpenGL Operation" << endl;
				break;
			case GL_INVALID_ENUM:
				cerr << "Invalid OpenGL ENUM" << endl;
				break;
			case GL_INVALID_VALUE:
				cerr << "Invalid OpenGL Value" << endl;
				break;
			case GL_OUT_OF_MEMORY:
				cerr << "OpenGL out of memory" << endl;
				break;
			case GL_INVALID_FRAMEBUFFER_OPERATION:
				cerr <<"Invalid OpenGL Framebuffer operation" << endl;
				break;
		}
		return true;
	}
	return false;
}

ShaderProgram::ShaderProgram(string vertex, string fragment){
	GLint linkOK = GL_FALSE;

	if((vs = compileShader(vertex,GL_VERTEX_SHADER)) == 0)
		cerr << "failed to compile vertex shader" << endl;
	if((fs = compileShader(fragment,GL_FRAGMENT_SHADER)) == 0)
		cerr << "failed to compile fragment shader" << endl;

	programID = glCreateProgram();
	glAttachShader(programID,vs);
	glAttachShader(programID,fs);
	glLinkProgram(programID);
	glGetProgramiv(programID,GL_LINK_STATUS,&linkOK);
	if(!linkOK){
		cerr << "Error in linking shader program" << endl;
		printLog(programID);
		good = false;
		return;
	}

	good = true;
}

ShaderProgram::~ShaderProgram(){
	glDeleteShader(vs);
	glDeleteShader(fs);
	glDeleteProgram(programID);
}

int ShaderProgram::compileShader(string filename, GLenum type){
	string data = fileToStr(filename);
	const char *sourcec = data.c_str();

	GLuint shader = glCreateShader(type);
	glShaderSource(shader,1,&sourcec, NULL);
	glCompileShader(shader);
	GLint compileOK = GL_FALSE;
	glGetShaderiv(shader,GL_COMPILE_STATUS,&compileOK);
	if(compileOK == GL_FALSE){
		cerr << filename << ":" << endl; 
		cerr << sourcec << endl;
		printLog(shader);
		return 0;
	}

	return shader;
}

GLuint ShaderProgram::getID(){
	return programID;
}

GLint ShaderProgram::getAttribute(int pos){
	return attributes.at(pos);
}

GLint ShaderProgram::getAttribute(string name){
	const char *cstr = name.c_str();
	GLint attribute = glGetAttribLocation(programID,cstr);
	if(attribute == -1){
		cerr << "Could not find attribute " << name << endl;
		return 0;
	}

	return attribute;
}

GLint ShaderProgram::setAttribute(string name){
	const char *cstr = name.c_str();
	GLint attribute = glGetAttribLocation(programID,cstr);
	if(attribute == -1){
		cerr << "Could not find attribute " << name << endl;
		return 0;
	}

	attributes.push_back(attribute);
	return attribute;
}

GLint ShaderProgram::getUniform(int pos){
	return uniforms.at(pos);
}

GLint ShaderProgram::getUniform(string name){
	const char *cstr = name.c_str();
	GLint uniform = glGetUniformLocation(programID,cstr);
	if(uniform == -1){
		cerr << "Could not find uniform " << name << endl;
		return 0;
	}

	return uniform;
}

GLint ShaderProgram::setUniform(string name){
	const char *cstr = name.c_str();
	GLint uniform = glGetUniformLocation(programID,cstr);
	if(uniform == -1){
		cerr << "Could not find uniform " << name << endl;
		return 0;
	}

	uniforms.push_back(uniform);
	return uniform;
}
