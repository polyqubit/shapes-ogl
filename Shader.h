#pragma once
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
class Shader
{
public:
	// the program ID
	GLuint ID;
	// constructor reads and builds the shader
	Shader(void);
	Shader(const char* vertexPath, const char* fragmentPath);
	// use/activate the shader
	void use();
	// delete shader + program
	void destroy();
	// utility uniform functions
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
};