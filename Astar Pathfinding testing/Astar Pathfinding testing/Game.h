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
	vector<Tile*> Pathfind();
	int GetDistanceCost(Tile *tile1, Tile *tile2);
	int CheckPathLength(Tile *tile, int oldLength);
};

#endif