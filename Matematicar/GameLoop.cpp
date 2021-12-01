#include "GameLoop.h"
#include <iostream>
#include <fstream>

GameLoop::GameLoop() {
	_window = nullptr;
	_width = 1024;
	_height = 768;
	_gameState = GAME_STATE::PLAY;
	_renderer = nullptr;
	_playerPos = glm::vec2(0.0f, 0.0f);
	_time = 0;
	_keyID[0] = false;
	TILE_WIDTH = 0.2f;
	_vrata = false;
	_jelGotovo = false;
	_kojiLevel = 3;
	_biloKTP = 0;
	_mrtav = 0;
	_biloKTP = 0;
	for (int i = 0; i < 10; i++) {
		_daliJeGore[i] = 0;
		_poluga[i] = 0;
	}
	
}


GameLoop::~GameLoop() {
	SDL_DestroyRenderer(_renderer);
	SDL_DestroyWindow(_window);
}

void GameLoop::run() {
	initSystem();
	initShader();
	gameLoop();     
}

void GameLoop::initSystem() {
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	_window = SDL_CreateWindow("Matematicar", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _width, _height, SDL_WINDOW_OPENGL);
	SDL_GLContext glContext = SDL_GL_CreateContext(_window);
	glewExperimental = true;
	glewInit();
	SDL_GL_SetSwapInterval(1);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
	std::printf("OpenGL Verzija: %s\n", glGetString(GL_VERSION));
	_playerSpeed = 0.01f;
	_didLoad = false;
}

void GameLoop::gameLoop() {
	while (_gameState == GAME_STATE::PLAY) {
		getInput();
		draw();
		_time += 0.004f;
	}
	while (_gameState == GAME_STATE::EXIT) {
		getInput();
		if (_biloKTP) {
			break;
		}
		if (_mrtav == true) {
			glClearDepth(1.0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glClearColor(0.9f, 0.6f, 0.5f, 1.0f);
			_shader.draw(-1.0f, -1.0f, 2.0f, 2.0f, 7);
			SDL_GL_SwapWindow(_window);
		}
		else {
			break;
		}
	}
}

void GameLoop::draw() {
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.9f, 0.6f, 0.5f, 1.0f);
	//std::cout << _normMouseCoordX << " " << _normMouseCoordY << std::endl;
	if (_keyID[SDLK_w]) _playerPos.y += _playerSpeed * _collide[0];
	if (_keyID[SDLK_s]) _playerPos.y -= _playerSpeed * _collide[1];
	if (_keyID[SDLK_a]) _playerPos.x -= _playerSpeed * _collide[2];
	if (_keyID[SDLK_d]) _playerPos.x += _playerSpeed * _collide[3];

	if (_kojiLevel == 1) {
		if (!_jelGotovo) {
			if (_keyID[SDL_BUTTON_LEFT] == true && _normMouseCoordX >= _clickable[0].x && _normMouseCoordX <= _clickable[0].x + TILE_WIDTH && _normMouseCoordY <= _clickable[0].y + TILE_WIDTH && _normMouseCoordY >= _clickable[0].y) {
				_vrata = true;
				_daliJeGore[0] = 1;
			}

			if (_playerPos.x + TILE_WIDTH >= _vrataPos.x && _playerPos.x + TILE_WIDTH <= _vrataPos.x + TILE_WIDTH && _playerPos.y + TILE_WIDTH <= _vrataPos.y + TILE_WIDTH && _playerPos.y + TILE_WIDTH >= _vrataPos.y - _playerSpeed && _vrata == true) {
				_jelGotovo = true;
			}
			_shader.draw(_playerPos.x, _playerPos.y, TILE_WIDTH, TILE_WIDTH, 0, glm::vec3(glm::cos(_time), 1.5 * glm::sin(_time), -glm::cos(_time)));

			loadLevel("Levels/level1.txt");
		}
		else {
			_shader.draw(-1.0f, -1.0f, 2.0f, 2.0f, 6);
			if (_keyID[SDL_BUTTON_LEFT]) {
				if (_normMouseCoordX >= -0.41 && _normMouseCoordX <= -0.29 && _normMouseCoordY <= 0.23 && _normMouseCoordY >= 0.14) {
					resetLevel();
					
				}
				else {
					_mrtav = true;
					_gameState = GAME_STATE::EXIT;
				}
			}
		}

	}
	else if (_kojiLevel == 2) {
		if (!_jelGotovo) {
			for (int i = 0; i < 3; i++) {
				if (_keyID[SDL_BUTTON_LEFT] == true && _normMouseCoordX >= _clickable[i].x && _normMouseCoordX <= _clickable[i].x + TILE_WIDTH && _normMouseCoordY <= _clickable[i].y + TILE_WIDTH && _normMouseCoordY >= _clickable[i].y) {
					_daliJeGore[i] = 1;
					if (_daliJeGore[0] && _daliJeGore[1] && _daliJeGore[2]) _vrata = true;
				}
			}

			loadLevel("Levels/level2.txt");
			_shader.draw(_playerPos.x, _playerPos.y, TILE_WIDTH, TILE_WIDTH, 0, glm::vec3(glm::cos(_time), 1.5 * glm::sin(_time), -glm::cos(_time)));

			if (_playerPos.x + TILE_WIDTH >= _vrataPos.x && _playerPos.x + TILE_WIDTH <= _vrataPos.x + TILE_WIDTH && _playerPos.y <= _vrataPos.y + TILE_WIDTH + _playerSpeed && _playerPos.y >= _vrataPos.y && _vrata == true) {
				_jelGotovo = true;
			}
			
		}
		else {
			_shader.draw(-1.0f, -1.0f, 2.0f, 2.0f, 9);
			if (_keyID[SDL_BUTTON_LEFT]) {
				if (_normMouseCoordX >= -0.06 && _normMouseCoordX <= 0.13 && _normMouseCoordY >= 0.06 && _normMouseCoordY <= 0.18) {
					resetLevel();
				}
				else {
					_mrtav = true;
					_gameState = GAME_STATE::EXIT;
				}
			}
		}
	}
	else if (_kojiLevel == 3) {
		if (!_jelGotovo) {
			loadLevel("Levels/level3.txt");

			if (_playerPos.x + TILE_WIDTH >= _plate[0].x - _playerSpeed && _playerPos.x + TILE_WIDTH <= _plate[0].x + TILE_WIDTH && _playerPos.y <= _plate[0].y + TILE_WIDTH + _playerSpeed && _playerPos.y >= _plate[0].y) {
				_isOnP = true;
			}
			else if (_playerPos.x <= _plate[0].x + TILE_WIDTH + _playerSpeed && _playerPos.x >= _plate[0].x && _playerPos.y >= _plate[0].y && _playerPos.y <= _plate[0].y + TILE_WIDTH + _playerSpeed) {
				_isOnP = true;
			}
			//gore desno
			else if (_playerPos.x + TILE_WIDTH >= _plate[0].x - _playerSpeed && _playerPos.x + TILE_WIDTH <= _plate[0].x + TILE_WIDTH + _playerSpeed && _playerPos.y + TILE_WIDTH >= _plate[0].y - _playerSpeed && _playerPos.y + TILE_WIDTH <= _plate[0].y + TILE_WIDTH + _playerSpeed) {
				_isOnP = true;
			}
			//gore lijevo
			else if (_playerPos.x <= _plate[0].x + TILE_WIDTH + _playerSpeed && _playerPos.x >= _plate[0].x && _playerPos.y + TILE_WIDTH >= _plate[0].y - _playerSpeed && _playerPos.y + TILE_WIDTH <= _plate[0].y + TILE_WIDTH + _playerSpeed) {
				_isOnP = true;
			}
			else if (_normMouseCoordX >= _plate[0].x && _normMouseCoordX <= _plate[0].x + TILE_WIDTH && _normMouseCoordY >= _plate[0].y && _normMouseCoordY <= _plate[0].y + TILE_WIDTH) {
				_isOnP = true;
			}
			else {
				_isOnP = false;
			}
			if (_isOnP) {
				_vrata = true;
			}
			else {
				_vrata = false;
			}
			
			if (_playerPos.x + TILE_WIDTH >= _vrataPos.x - _playerSpeed && _playerPos.x + TILE_WIDTH <= _vrataPos.x + TILE_WIDTH + _playerSpeed && _playerPos.y + TILE_WIDTH >= _vrataPos.y - _playerSpeed && _playerPos.y + TILE_WIDTH <= _vrataPos.y + TILE_WIDTH + _playerSpeed) {
				_jelGotovo = true;
			}

			_shader.draw(_playerPos.x, _playerPos.y, TILE_WIDTH, TILE_WIDTH, 0, glm::vec3(1.5 * glm::cos(_time), glm::sin(_time), -glm::cos(_time)));
		}
		else {    
			_shader.draw(-1.0f, -1.0f, 2.0f, 2.0f, 11);
			if (_keyID[SDL_BUTTON_LEFT]) {
				if (_normMouseCoordX >= -0.51 && _normMouseCoordX <= -0.34 && _normMouseCoordY >= 0.04 && _normMouseCoordY <= 0.17) {
					resetLevel();
				}
				else {
					_mrtav = true;
					_gameState = GAME_STATE::EXIT;
				}
			}
		}
	}

	for (int i = 0; i < 4; i++) _collide[i] = 1;
	checkIfColliding();
	SDL_GL_SwapWindow(_window);
}

void GameLoop::getInput() {
	SDL_Event _event;
	while (SDL_PollEvent(&_event)) {
		switch (_event.type) {
			case SDL_QUIT:
				_gameState = GAME_STATE::EXIT;
				break;
			case SDL_MOUSEMOTION:
				_normMouseCoordX = -1.0 + 2.0 * (double)_event.motion.x / _width;
				_normMouseCoordY = 1.0 - 2.0 * (double)_event.motion.y / _height;
				break;
			case SDL_KEYDOWN:
				_keyID[_event.key.keysym.sym] = true;
				_biloKTP = true;
				break;
			case SDL_KEYUP:
				_keyID[_event.key.keysym.sym] = false;
				_biloKTP = false;
				break;
			case SDL_MOUSEBUTTONDOWN:
				_keyID[_event.button.button] = true;
				break;
			case SDL_MOUSEBUTTONUP:
				_keyID[_event.button.button] = false;
				break;
		}
	}
}

void GameLoop::initShader() {
	_shader.init();
	_shader.compile("Shaders/shader.vert", "Shaders/shader.frag");

	_shader.addImg("src/igrac.png"); 
	_shader.addImg("src/zid.png"); 
	_shader.addImg("src/polugaGore.png"); 
	_shader.addImg("src/polugaDolje.png");
	_shader.addImg("src/vrata_zatvorena.png");
	_shader.addImg("src/vrata_otvorena.png");
	_shader.addImg("src/text.png");
	_shader.addImg("src/GMover.png");
	_shader.addImg("src/MainMenu.png");
	_shader.addImg("src/text1.png");
	_shader.addImg("src/Plate.png");
	_shader.addImg("src/text2.png");
}

void GameLoop::loadLevel(std::string filePath) {
	if (!_didLoad) {
		std::ifstream file(filePath);
		if (file.fail()) {
			std::printf("Datoteka %s se nije uspjela otvoriti! :(\n", filePath.c_str());
		}
		std::string line = "";
		while (std::getline(file, line)) {
			_level.push_back(line);
		}
		file.close();
		
	}
	int polugaCont = 0;
	for (unsigned int y = 0; y < _level.size(); y++) {
		for (unsigned int x = 0; x < _level[y].size(); x++) {
			switch (_level[y][x]) {
			case 'W':
				_shader.draw(-1.0f + (x * TILE_WIDTH), 0.8f - (y * TILE_WIDTH), TILE_WIDTH, TILE_WIDTH, 1);
				if(!_didLoad) _zapreka.push_back(glm::vec2(-1.0f + (x * TILE_WIDTH), 0.8f - (y * TILE_WIDTH)));
				break;
			case 'P':
				if(_daliJeGore[polugaCont] == 0) _shader.draw(-1.0f + (x * TILE_WIDTH), 0.8f - (y * TILE_WIDTH), TILE_WIDTH, TILE_WIDTH, 2);
				else _shader.draw(-1.0f + (x * TILE_WIDTH), 0.8f - (y * TILE_WIDTH), TILE_WIDTH, TILE_WIDTH, 3);
				if (!_didLoad) { _zapreka.push_back(glm::vec2(-1.0f + (x * TILE_WIDTH), 0.8f - (y * TILE_WIDTH))); _clickable.push_back(glm::vec2(-1.0f + (x * TILE_WIDTH), 0.8f - (y * TILE_WIDTH))); }
				polugaCont++;
				break;
			case 'V':
				if(!_vrata) _shader.draw(-1.0f + (x * TILE_WIDTH), 0.8f - (y * TILE_WIDTH), TILE_WIDTH, TILE_WIDTH, 4);
				else _shader.draw(-1.0f + (x * TILE_WIDTH), 0.8f - (y * TILE_WIDTH), TILE_WIDTH, TILE_WIDTH, 5);
				if (!_didLoad) _zapreka.push_back(glm::vec2(-1.0f + (x * TILE_WIDTH), 0.8f - (y * TILE_WIDTH)));
				_vrataPos = glm::vec2(-1.0f + (x * TILE_WIDTH), 0.8f - (y * TILE_WIDTH));
				break;
			case 'L':
				_shader.draw(-1.0f + (x * TILE_WIDTH), 0.8f - (y * TILE_WIDTH), TILE_WIDTH, TILE_WIDTH, 10);
				if (!_didLoad) _plate.push_back(glm::vec2(-1.0f + (x * TILE_WIDTH), 0.8f - (y * TILE_WIDTH)));
				break;
			case '@':
				if (!_didLoad) _playerPos = glm::vec2(-1.0f + (x * TILE_WIDTH), 0.8f - (y * TILE_WIDTH));
				break;
			case '.':
				break;
			default:
				std::printf("Char %c nije pronadjen u %s\n", _level[y][x], filePath.c_str());
				break;
			}
		}
	}
	_didLoad = true;
	
}

void GameLoop::checkIfColliding() {
	bool collideSide[4] = { false, false, false, false };
	for (int i = 0; i < _zapreka.size(); i++) {
		//dolje lijevo
		if (_playerPos.x <= _zapreka[i].x + TILE_WIDTH + _playerSpeed && _playerPos.x  >= _zapreka[i].x && _playerPos.y >= _zapreka[i].y && _playerPos.y <= _zapreka[i].y + TILE_WIDTH + _playerSpeed) {
			collideSide[0] = true;
		}
		//dolje desno
		else if (_playerPos.x + TILE_WIDTH >= _zapreka[i].x - _playerSpeed && _playerPos.x + TILE_WIDTH <= _zapreka[i].x + TILE_WIDTH && _playerPos.y <= _zapreka[i].y + TILE_WIDTH + _playerSpeed && _playerPos.y >= _zapreka[i].y) {
			collideSide[1] = true;
		}
		//gore desno
		else if (_playerPos.x + TILE_WIDTH >= _zapreka[i].x - _playerSpeed && _playerPos.x + TILE_WIDTH <= _zapreka[i].x + TILE_WIDTH + _playerSpeed && _playerPos.y + TILE_WIDTH >= _zapreka[i].y - _playerSpeed && _playerPos.y + TILE_WIDTH <= _zapreka[i].y + TILE_WIDTH + _playerSpeed) {
			collideSide[2] = true;
		}
		//gore lijevo
		else if (_playerPos.x <= _zapreka[i].x + TILE_WIDTH + _playerSpeed && _playerPos.x >= _zapreka[i].x && _playerPos.y + TILE_WIDTH >= _zapreka[i].y - _playerSpeed && _playerPos.y + TILE_WIDTH <= _zapreka[i].y + TILE_WIDTH + _playerSpeed) {
			collideSide[3] = true;
		}
	}
	if (collideSide[0]) {
		if (collideSide[1]) {
			if (collideSide[2]) {
				_collide[3] = 0;
			}
			else if (collideSide[3]) {
				_collide[2] = 0;
			}
			_collide[1] = 0;
		}
		else if (collideSide[3]) {
			if (collideSide[2]) {
				_collide[0] = 0;
			}
			_collide[2] = 0;
		}  
		else {
			_collide[1] = 0;
			_collide[2] = 0;
		}
	}
	else if (collideSide[1]) {
		if (collideSide[2]) {
			if (collideSide[3]) {
				_collide[0] = 0;
			}
			_collide[3] = 0;
		}
		else {
			_collide[1] = 0;
			_collide[3] = 0;
		}
	}
	else if (collideSide[2]) {
		if (collideSide[3]) {
			_collide[0] = 0;
		}
		else {
			_collide[0] = 0;
			_collide[3] = 0;
		}
	}
	else if (collideSide[3]) {
		_collide[2] = 0;
		_collide[0] = 0;
	}

	for (int i = 0; i < 4; i++) {
		collideSide[i] = false;
	}








	
}

void GameLoop::resetLevel() {
	_kojiLevel++;
	_jelGotovo = false;
	_zapreka.clear();
	_clickable.clear();
	_vrata = false;
	_didLoad = false;
	_level.clear();
	for (int i = 0; i < 10; i++) {
		_poluga[i] = 0;
		_daliJeGore[i] = 0;
	}
	_plate.clear();
}