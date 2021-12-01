#pragma once
#include <GLEW/glew.h>
#include <string>
#include <vector>
#include <array>
#include <GLM/glm.hpp>
#include <map>

class Shader {
public:
	Shader();
	~Shader();
	void init();
	void compile(std::string vertexFile, std::string fragmentFile);
	void draw(GLfloat x, GLfloat y, GLfloat width, GLfloat height, int texture, glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f));
	void addImg(std::string texPath);
private:
	GLuint _programID;
	GLuint _vertexShaderID;
	GLuint _fragmentShaderID;
	GLuint _vao;
	GLuint _vbo;
	GLuint _ebo;
	GLuint _tex;
	GLubyte *_image[30];
	int _width[30];
	int _height[30];
	int _imgCounter;
};

