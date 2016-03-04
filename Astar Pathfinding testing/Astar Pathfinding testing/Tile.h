#ifndef TILE_H
#define TILE_H

#include <SFML/Graphics.hpp>

using namespace std;

class Tile
{
public:
	Tile(int type, sf::Vector2f position);
	~Tile();
	static void Initialize(sf::RenderWindow *mainWindow, int size);
	void Update();
	void Render();
	void SetNeighbor(Tile *tile);
	void SetOccupied(bool occupied);
	int GetType();
	vector<Tile*> GetNeighbors();
	bool GetOccupied();
private:
	sf::Vector2f mPosition;
	int mType = 0;
	vector<Tile*> mNeighborsVector;
	sf::RectangleShape mShape;
	bool mOccupied = false;
};

#endif