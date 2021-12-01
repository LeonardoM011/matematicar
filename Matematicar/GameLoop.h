#pragma once
#include <SDL/SDL.h>
#include <GLEW/glew.h>
#include <map>
#include <string>
#include "Shader.h"

enum class GAME_STATE 
{
	PLAY,
	EXIT
};

class GameLoop {
public:
	GameLoop();
	~GameLoop();
	void run();
private:
	void gameLoop();
	void draw();
	void getInput();
	void initSystem();
	void initShader();
	void loadLevel(std::string filePath);
	void checkIfColliding();
	void resetLevel();
	SDL_Window *_window;
	int _height;
	int _width;
	float TILE_WIDTH;
	GAME_STATE _gameState;
	SDL_Renderer *_renderer;
	Shader _shader;
	std::map<GLuint, bool> _keyID;
	std::vector<std::string> _level;
	glm::vec2 _playerPos;
	float _playerSpeed;
	bool _didLoad;
	double _normMouseCoordX;
	double _normMouseCoordY;
	float _time;
	std::vector<glm::vec2> _zapreka;
	std::vector<glm::vec2> _clickable;
	std::vector<glm::vec2> _plate;
	bool _vrata;
	glm::vec2 _vrataPos;
	bool _jelGotovo;
	int _kojiLevel;
	char _collide[4];
	char _poluga[10];
	char _daliJeGore[10];
	bool _mrtav;
	bool _biloKTP;
	bool _isOnP;
};

