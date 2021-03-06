#ifndef TILE_H
#define TILE_H

#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;

struct PathValues
{
	int gCost, //Distance from starting Node.
		hCost, //Distance from end Node.
		fCost; //gCost + hCost.
};

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
	void SetStart(bool start);
	void SetGoal(bool goal);
	void SetPathValues(int gCost, int hCost);
	void SetPathParent(Tile *parent);
	void SetPath(bool path);
	int GetType();
	vector<Tile*> GetNeighbors();
	bool GetOccupied();
	bool GetMouseOver(sf::Vector2i mousePosition);
	sf::Vector2i GetGridPosition();
	sf::Vector2i GetHexGridPosition();
	PathValues GetPathValues();
	Tile* GetPathParent();
	vector<Tile*> GetPath(vector<Tile*> path);
private:
	sf::Vector2f mPosition;
	sf::Vector2i mGridPosition, mHexGridPosition;
	int mType = 0;
	vector<Tile*> mNeighborsVector;
	sf::RectangleShape mShape;
	bool mOccupied = false, mStart = false, mGoal = false, mPath = false;
	sf::IntRect mHitBox;
	PathValues mPathValues;
	Tile* mPathParent;
};

#endif