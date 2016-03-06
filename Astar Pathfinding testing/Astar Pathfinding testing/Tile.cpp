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
	mHitBox.height = tileSize;
	mHitBox.width = tileSize;
	mHitBox.left = mPosition.x;
	mHitBox.top = mPosition.y;
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
	mShape.setPosition(mPosition);
	mHitBox.top = mPosition.y;
}

void Tile::Render()
{
	window->draw(mShape);
}

void Tile::ClearNeighbors()
{
	mNeighborsVector.clear();
}

void Tile::SetType(int type)
{
	mType = type;
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
}

void Tile::SetNeighbor(Tile *tile)
{
	mNeighborsVector.push_back(tile);
}

void Tile::SetOccupied(bool occupied)
{
	mOccupied = occupied;
}

void Tile::SetPosition(sf::Vector2f newPosition)
{
	mPosition = newPosition;
}

int Tile::GetType() { return mType; }

vector<Tile*> Tile::GetNeighbors() { return mNeighborsVector; }

bool Tile::GetOccupied() { return mOccupied; }

bool Tile::GetMouseOver(sf::Vector2i mousePosition)
{
	return mHitBox.contains(mousePosition);
}