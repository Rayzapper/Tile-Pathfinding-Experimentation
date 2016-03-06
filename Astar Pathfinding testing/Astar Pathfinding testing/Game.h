#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <iostream>

#include "Tile.h"
#include "Entity.h"

using namespace std;

class Game
{
public:
	Game();
	~Game();
	void Run();
private:
	void Initialize();
	void Update();
	void Render();
	void Setup();
	void GridChange(int type);
};

#endif