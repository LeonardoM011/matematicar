#include "Shader.h"
#include <fstream>
#include <cstdio>
#include <SOIL/SOIL.h>

Shader::Shader() {
	_vertexShaderID = 0;
	_fragmentShaderID = 0;
	_programID = 0;
	_vbo = 0;
	_vao = 0;
	_tex = 0;
	_ebo = 0;
	_imgCounter = 0;
	for (int i = 0; i < 30; i++) _image[i] = nullptr;
}


Shader::~Shader() {
	glDeleteTextures(1, &_tex);

	glDeleteProgram(_programID);
	glDeleteShader(_fragmentShaderID);
	glDeleteShader(_vertexShaderID);

	glDeleteBuffers(1, &_ebo);
	glDeleteBuffers(1, &_vbo);

	glDeleteVertexArrays(1, &_vao);
	for (int i = 0; i < 10; i++) {
		SOIL_free_image_data(_image[i]);
	}
}

void Shader::init() {
	_programID = glCreateProgram();
	_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_vbo);
	glGenBuffers(1, &_ebo);
	glGenTextures(1, &_tex);
}

void Shader::addImg(std::string imgPath) {
	_image[_imgCounter] = SOIL_load_image(imgPath.c_str(), &_width[_imgCounter], &_height[_imgCounter], 0, SOIL_LOAD_RGB);
	_imgCounter++;
}

void Shader::draw(GLfloat x, GLfloat y, GLfloat width, GLfloat height, int texture, glm::vec3 color) {
	glBindVertexArray(_vao);

	GLfloat vertices[28];
	vertices[0] = x;
	vertices[1] = y;
	vertices[2] = color.r;
	vertices[3] = color.g;
	vertices[4] = color.b;
	vertices[5] = 0.0f; // texcoord
	vertices[6] = 1.0f;

	vertices[7] = x + width;
	vertices[8] = y;
	vertices[9] = color.r;
	vertices[10] = color.g;
	vertices[11] = color.b;
	vertices[12] = 1.0f;
	vertices[13] = 1.0f;

	vertices[14] = x + width;
	vertices[15] = y + height;
	vertices[16] = color.r;
	vertices[17] = color.g;
	vertices[18] = color.b;
	vertices[19] = 1.0f;
	vertices[20] = 0.0f;

	vertices[21] = x;
	vertices[22] = y + height;
	vertices[23] = color.r;
	vertices[24] = color.g;
	vertices[25] = color.b;
	vertices[26] = 0.0f;
	vertices[27] = 0.0f;

	GLuint elements[] = {
		0, 1, 2,
		2, 3, 0
	};

	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

	// INPUT
	GLint posAttrib = glGetAttribLocation(_programID, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), 0);

	GLint colAttrib = glGetAttribLocation(_programID, "color");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));

	GLint texAttrib = glGetAttribLocation(_programID, "texcoord");
	glEnableVertexAttribArray(texAttrib);
	glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(5 * sizeof(GLfloat)));

	
	glBindTexture(GL_TEXTURE_2D, _tex);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width[texture], _height[texture], 0, GL_RGB, GL_UNSIGNED_BYTE, _image[texture]);
	glUniform1i(glGetUniformLocation(_programID, "tex"), 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Shader::compile(std::string vertexFile, std::string fragmentFile) {
	

	std::ifstream file(vertexFile);
	if (file.fail()) {
		std::printf("Datoteka %s se nije uspjela otvoriti! :(\n", vertexFile.c_str());
	}
	std::string shaderCont = "";
	std::string line = "";
	while (std::getline(file, line)) {
		shaderCont += line + "\n";
	}
	file.close();

	const char* shaderCont2 = shaderCont.c_str();

	glShaderSource(_vertexShaderID, 1, &shaderCont2, NULL);
	glCompileShader(_vertexShaderID);
	int success = 0;
	glGetShaderiv(_vertexShaderID, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE) {
		GLint maxLength = 0;
		glGetShaderiv(_vertexShaderID, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<char> errorLog(maxLength);
		glGetShaderInfoLog(_vertexShaderID, maxLength, &maxLength, &errorLog[0]);
		glDeleteShader(_vertexShaderID);
		std::printf("%s\n", &(errorLog[0]));
		std::printf("Vertex shader se nije uspio kompalirati! :(\n");
	}

	

	std::ifstream file2(fragmentFile);
	if (file2.fail()) {
		std::printf("Datoteka %s se nije uspjela otvoriti! :(\n", fragmentFile.c_str());
	}
	shaderCont = "";
	line = "";
	while (std::getline(file2, line)) {
		shaderCont += line + "\n";
	}
	file.close();

	const char* shaderCont3 = shaderCont.c_str();

	glShaderSource(_fragmentShaderID, 1, &shaderCont3, NULL);
	glCompileShader(_fragmentShaderID);

	success = 0;
	glGetShaderiv(_fragmentShaderID, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE) {
		GLint maxLength = 0;
		glGetShaderiv(_fragmentShaderID, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<char> errorLog(maxLength);
		glGetShaderInfoLog(_fragmentShaderID, maxLength, &maxLength, &errorLog[0]);
		glDeleteShader(_fragmentShaderID);
		std::printf("%s\n", &(errorLog[0]));
		std::printf("Fragment shader se nije uspio kompalirati! :(\n");
	}

	glAttachShader(_programID, _vertexShaderID);
	glAttachShader(_programID, _fragmentShaderID);

	// OUTPUT
	glBindFragDataLocation(_programID, 0, "outColor");


	glLinkProgram(_programID);
	glUseProgram(_programID);
}