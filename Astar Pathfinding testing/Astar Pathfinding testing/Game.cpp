#include "Game.h"

static sf::RenderWindow *window;
static sf::View *mainView;
static const int screenWidth = 1000, screenHeight = 800, tileSize = 32, mapSizeX = 20, mapSizeY = 20;
static bool focus = true;

typedef vector<Tile*> TileRow;
typedef vector<TileRow> TileMap;
static TileMap tileMap;
typedef vector<Entity*> EntityVector;
static EntityVector entityVector;

static sf::Vector2i mousePosition;

sf::Font font;

Game::Game()
{
	window = new sf::RenderWindow(sf::VideoMode(screenWidth, screenHeight), "Pathfinding Testing");
	mainView = new sf::View(sf::Vector2f(screenWidth / 2, screenHeight / 2), sf::Vector2f(screenWidth, screenHeight));
	window->setVerticalSyncEnabled(true);
	window->setFramerateLimit(60);
	Initialize();
	Setup();
}

Game::~Game()
{

}

void Game::Run()
{
	while (window->isOpen())
	{
		sf::Event event;
		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window->close();
			if (event.type == sf::Event::GainedFocus)
				focus = true;
			if (event.type == sf::Event::LostFocus)
				focus = false;
		}
		if (focus)
		{
			Update();
			Render();
		}
		else
		{
			sf::Time time = sf::milliseconds(10);
			sf::sleep(time);
		}
	}
}

void Game::Initialize()
{
	Tile::Initialize(window, tileSize);
	Entity::Initialize(window);
}

void Game::Update()
{

}

void Game::Render()
{
	sf::Color windowColor(100, 100, 100, 255);
	window->clear(windowColor);
	window->setView(*mainView);
	for (TileMap::size_type y = 0; y < tileMap.size(); y++)
	{
		for (TileRow::size_type x = 0; x < tileMap[y].size(); x++)
		{
			tileMap[y][x]->Render();
		}
	}
	window->display();
}

void Game::Setup()
{
	/*
	Însert queries for types of grids and so forth here.
	*/
	for (int y = 0; y < mapSizeY; y++)
	{
		TileRow row;
		for (int x = 0; x < mapSizeX; x++)
		{
			Tile *tile;
			tile = new Tile(0, sf::Vector2f(x * tileSize, y * tileSize));
			row.push_back(tile);
		}
		tileMap.push_back(row);
	}
}