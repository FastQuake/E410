#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include "shaderutils.hpp"

GLuint programShader;

void print_glm(glm::mat4 m)
{
    std::cout << std::setw(2) << std::setprecision(3) << "[" << m[0].x << "," << m[0].y << "," << m[0].z << "," << m[0].w << "]\n";
    std::cout << std::setw(2) << std::setprecision(3) << "[" << m[1].x << "," << m[1].y << "," << m[1].z << "," << m[1].w << "]\n";
    std::cout << std::setw(2) << std::setprecision(3) << "[" << m[2].x << "," << m[2].y << "," << m[2].z << "," << m[2].w << "]\n";
    std::cout << std::setw(2) << std::setprecision(3) << "[" << m[3].x << "," << m[3].y << "," << m[3].z << "," << m[3].w << "]\n";
}

int initResources(){
	GLint linkOK = GL_FALSE;

	//Create a vertex shader
	GLuint vs = createShader("vertex.glsl",GL_VERTEX_SHADER);
	if(vs == 0){
		return 0;
	}

	//Create a fragment shader
	GLuint fs = createShader("fragment.glsl",GL_FRAGMENT_SHADER);
	if(fs == 0){
		return 0;
	}

	//Create program by link shaders together
	programShader = glCreateProgram();
	glAttachShader(programShader, vs);
	glAttachShader(programShader, fs);
	glLinkProgram(programShader);
	glGetProgramiv(programShader,GL_LINK_STATUS,&linkOK);
	if(linkOK == GL_FALSE){
		cerr << "Error in linking program" << endl;
		printLog(programShader);
		return 0;
	}
	/*depthShader = glCreateProgram();
	GLuint vs_s = createShader("fragment.glsl",GL_VERTEX_SHADER);
	GLuint fs_s = createShader("shadfrag.glsl",GL_FRAGMENT_SHADER);
	glAttachShader(depthShader,vs_s);
	glAttachShader(depthShader,fs_s);
	glGetProgramiv(depthShader,GL_LINK_STATUS,&linkOK);
		if(linkOK == GL_FALSE){
			cerr << "Error in linking program" << endl;
			printLog(depthShader);
			return 0;
		}*/
	return 1;
}

GLenum createShader(string filename, GLenum type){
	string source = loadFile(filename);
	cout << "filename: " <<filename << endl <<source << endl;
	const char *sourcec = source.c_str();

	GLuint shader = glCreateShader(type);
	glShaderSource(shader,1,&sourcec,NULL);
	glCompileShader(shader);
	GLint compileOK = false;
	glGetShaderiv(shader,GL_COMPILE_STATUS,&compileOK);
	if(compileOK == GL_FALSE){
		cerr << filename << ": " << endl;
		cerr << sourcec << endl;
		printLog(shader);
		glDeleteShader(shader);
		return 0;
	}
	return shader;
}

GLuint getAttribute(string name){
	const char *cstr = name.c_str();
	cout << "attrib " << name << endl;
	GLint attribute = glGetAttribLocation(programShader,cstr);
	//std::cout << glGetError() << std::endl;
	if(attribute == -1){
		cerr << "Could not find attribute " << name << endl;
		return 0;
	}
	return attribute;
}

GLuint getUniform(string name){
	const char *cstr = name.c_str();
	GLint uniform = glGetUniformLocation(programShader,cstr);
	//std::cout << glGetError() << std::endl;
	if(uniform == -1){
		cerr << "Could not find uniform " << name << endl;
		return 0;
	}
	return uniform;
}

void freeResources(){
	glDeleteProgram(programShader);
}

void printLog(GLuint object) {
    GLint log_length = 0;
    if(glIsShader(object))
        glGetShaderiv(object,GL_INFO_LOG_LENGTH,&log_length);
    else if(glIsProgram(object))
        glGetProgramiv(object,GL_INFO_LOG_LENGTH,&log_length);
    else {
        cerr << "printlog: Not a shader or a program" << endl;
        return;
    }

    char *log = new char[log_length];
    if(glIsShader(object))
        glGetShaderInfoLog(object,log_length,NULL,log);
    else if(glIsProgram(object))
        glGetProgramInfoLog(object,log_length,NULL,log);

    std::cerr << log << std::endl;
    delete log;
}

string loadFile(string filename){
	ifstream file(filename.c_str());
	stringstream buf;
	if(file.is_open()){
		buf << file.rdbuf();
	} else
		cerr << "Unable to open file: " << filename << endl;
	file.close();
	return buf.str();
}
