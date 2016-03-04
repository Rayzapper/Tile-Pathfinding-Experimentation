#include "Tile.h"

static sf::RenderWindow *window;
static int tileSize;

Tile::Tile(int type, sf::Vector2f position)
	:mType(type), mPosition(position)
{
	mShape.setSize(sf::Vector2f(tileSize, tileSize));
	sf::Color color, colorOutline;
	if (mType == 0)
	{
		color = sf::Color::White;
		colorOutline = sf::Color::Black;
	}
	else
	{
		color = sf::Color::Black;
		colorOutline = sf::Color::White;
	}
	mShape.setFillColor(color);
	mShape.setOutlineColor(colorOutline);
	mShape.setOutlineThickness(-1);
	mShape.setPosition(mPosition);
}

Tile::~Tile()
{

}

void Tile::Initialize(sf::RenderWindow *mainWindow, int size)
{
	window = mainWindow;
	tileSize = size;
}

void Tile::Update()
{

}

void Tile::Render()
{
	window->draw(mShape);
}

void Tile::SetNeighbor(Tile *tile)
{
	mNeighborsVector.push_back(tile);
}

void Tile::SetOccupied(bool occupied)
{
	mOccupied = occupied;
}

int Tile::GetType() { return mType; }

vector<Tile*> Tile::GetNeighbors() { return mNeighborsVector; }

bool Tile::GetOccupied() { return mOccupied; }