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
	void ClearNeighbors();
	void SetType(int type);
	void SetNeighbor(Tile *tile);
	void SetOccupied(bool occupied);
	void SetPosition(sf::Vector2f newPosition);
	int GetType();
	vector<Tile*> GetNeighbors();
	bool GetOccupied();
	bool GetMouseOver(sf::Vector2i mousePosition);
private:
	sf::Vector2f mPosition;
	int mType = 0;
	vector<Tile*> mNeighborsVector;
	sf::RectangleShape mShape;
	bool mOccupied = false;
	sf::IntRect mHitBox;
};

#endif