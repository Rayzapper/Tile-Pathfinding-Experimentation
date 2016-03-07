#include "Tile.h"

static sf::RenderWindow *window;
static int tileSize;

Tile::Tile(int type, sf::Vector2f position)
	:mType(type), mPosition(position)
{
	testFont.loadFromFile("Resources/calibri.ttf");
	mGridPosition.x = mPosition.x / tileSize;
	mGridPosition.y = mPosition.y / tileSize;
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

	testText.setColor(sf::Color::Black);
	testText.setPosition(mPosition.x, mPosition.y);
	testText.setFont(testFont);
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
	mStart = false;
	mGoal = false;
	testText.setPosition(mPosition.x, mPosition.y);
	testText.setString(to_string(testInt));
}

void Tile::Render()
{
	if (mStart)
		mShape.setFillColor(sf::Color::Yellow);
	else if (mGoal)
		mShape.setFillColor(sf::Color::Green);
	else if (mType == 0)
		SetType(0);
	else if (mType == 1)
		SetType(1);
	window->draw(mShape);
	window->draw(testText);
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

void Tile::SetStart(bool start)
{
	mStart = start;
}

void Tile::SetGoal(bool goal)
{
	mGoal = goal;
}

void Tile::SetPathValues(int gCost, int hCost)
{
	mPathValues.gCost = gCost;
	mPathValues.hCost = hCost;
	mPathValues.fCost = gCost + hCost;
}

void Tile::SetPathParent(Tile *parent)
{
	mPathParent = parent;
}

int Tile::GetType() { return mType; }

vector<Tile*> Tile::GetNeighbors() { return mNeighborsVector; }

bool Tile::GetOccupied() { return mOccupied; }

bool Tile::GetMouseOver(sf::Vector2i mousePosition) { return mHitBox.contains(mousePosition); }

sf::Vector2i Tile::GetGridPosition() { return mGridPosition; }

PathValues Tile::GetPathValues() { return mPathValues; }

Tile* Tile::GetPathParent() { return mPathParent; }