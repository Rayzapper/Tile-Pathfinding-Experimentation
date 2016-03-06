#include "Game.h"

static sf::RenderWindow *window;
static sf::View *mainView, *uiView;
static const int screenWidth = 1000, screenHeight = 656, tileSize = 32, mapSizeX = 20, mapSizeY = 20;
static bool focus = true, mouseReleased = true;

typedef vector<Tile*> TileRow;
typedef vector<TileRow> TileMap;
static TileMap tileMap;
static Tile *startTile, *targetTile;
typedef vector<Entity*> EntityVector;
static EntityVector entityVector;

static sf::Vector2i mousePosition;

sf::Font font;

struct Button
{
	sf::RectangleShape shape;
	sf::Text text;
	sf::IntRect box;
};

Button hexButton, fourButton, eightButton, stepButton, resetButton, wallButton, targetButton;

enum GameState
{
	WALLS,
	TARGETS
};

GameState gameState = WALLS;

Game::Game()
{
	window = new sf::RenderWindow(sf::VideoMode(screenWidth, screenHeight), "Pathfinding Testing");
	mainView = new sf::View(sf::Vector2f(screenWidth / 2, screenHeight / 2), sf::Vector2f(screenWidth, screenHeight));
	uiView = new sf::View(sf::Vector2f(screenWidth / 2, screenHeight / 2), sf::Vector2f(screenWidth, screenHeight));
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
	font.loadFromFile("Resources/calibri.ttf");
	Tile::Initialize(window, tileSize);
	Entity::Initialize(window);
}

void Game::Update()
{
	sf::Vector2i mousePosition = sf::Mouse::getPosition(*window);
	for (TileMap::size_type y = 0; y < tileMap.size(); y++)
	{
		for (TileRow::size_type x = 0; x < tileMap[y].size(); x++)
		{
			if (tileMap[y][x]->GetMouseOver(mousePosition))
			{
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
					if (gameState == WALLS)
						tileMap[y][x]->SetType(0);
					if (gameState == TARGETS && tileMap[y][x]->GetType() != 1)
					{
						startTile = tileMap[y][x];
					}
				}
				else if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
				{
					if (gameState == WALLS && tileMap[y][x] != startTile && tileMap[y][x] != targetTile)
						tileMap[y][x]->SetType(1);
					if (gameState == TARGETS && tileMap[y][x]->GetType() != 1)
					{
						targetTile = tileMap[y][x];
					}
				}
			}
			tileMap[y][x]->Update();
		}
	}
	if (startTile != nullptr)
		startTile->SetStart(true);
	if (targetTile != nullptr)
		targetTile->SetGoal(true);
	
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && mouseReleased)
	{
		mouseReleased = false;
		if (fourButton.box.contains(mousePosition))
			GridChange(0);
		else if (eightButton.box.contains(mousePosition))
			GridChange(1);
		else if (hexButton.box.contains(mousePosition))
			GridChange(2);
		else if (stepButton.box.contains(mousePosition))
		{
			
		}
		else if (resetButton.box.contains(mousePosition))
		{
			GridChange(0);
			for (TileMap::size_type y = 0; y < tileMap.size(); y++)
			{
				for (TileRow::size_type x = 0; x < tileMap[y].size(); x++)
				{
					tileMap[y][x]->SetType(0);
					startTile = nullptr;
					targetTile = nullptr;
				}
			}
		}
		else if (wallButton.box.contains(mousePosition))
			gameState = WALLS;
		else if (targetButton.box.contains(mousePosition))
			gameState = TARGETS;
	}
	if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
		mouseReleased = true;
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
	window->setView(*uiView);
	for (int i = 0; i < 7; i++)
	{
		Button *button;
		if (i == 0)
			button = &stepButton;
		else if (i == 1)
			button = &resetButton;
		else if (i == 2)
			button = &wallButton;
		else if (i == 3)
			button = &targetButton;
		else if (i == 4)
			button = &fourButton;
		else if (i == 5)
			button = &eightButton;
		else
			button = &hexButton;
		window->draw(button->shape);
		window->draw(button->text);
	}
	window->display();
}

void Game::Setup()
{
	//Create Tiles
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
	//Setup Neighbors
	for (TileMap::size_type y = 0; y < tileMap.size(); y++)
	{
		for (TileRow::size_type x = 0; x < tileMap[y].size(); x++)
		{
			if (y > 0)
				tileMap[y][x]->SetNeighbor(tileMap[y - 1][x]);
			if (x > 0)
				tileMap[y][x]->SetNeighbor(tileMap[y][x - 1]);
			if (x < tileMap[y].size() - 1)
				tileMap[y][x]->SetNeighbor(tileMap[y][x + 1]);
			if (y < tileMap.size() - 1)
				tileMap[y][x]->SetNeighbor(tileMap[y + 1][x]);
		}
	}

	//Setup UI
	for (int i = 0; i < 7; i++)
	{
		Button *button;
		if (i == 0)
		{
			button = &stepButton;
			button->text.setString("Step");
		}
		else if (i == 1)
		{
			button = &resetButton;
			button->text.setString("Reset");
		}
		else if (i == 2)
		{
			button = &wallButton;
			button->text.setString("Walls");
		}
		else if (i == 3)
		{
			button = &targetButton;
			button->text.setString("Targets");
		}
		else if (i == 4)
		{
			button = &fourButton;
			button->text.setString("Four");
		}
		else if (i == 5)
		{
			button = &eightButton;
			button->text.setString("Eight");
		}
		else
		{
			button = &hexButton;
			button->text.setString("Hex");
		}

		if (i > 3)
			button->shape.setFillColor(sf::Color::Red);
		else if (i < 2)
			button->shape.setFillColor(sf::Color::Black);
		else
			button->shape.setFillColor(sf::Color::Blue);
		button->shape.setPosition(sf::Vector2f(screenWidth - 300, 40 + i * 60));
		button->shape.setSize(sf::Vector2f(100, 50));
		button->text.setFont(font);
		button->text.setPosition(button->shape.getPosition());
		button->box.width = 100;
		button->box.height = 50;
		button->box.left = button->shape.getPosition().x;
		button->box.top = button->shape.getPosition().y;
	}
}

void Game::GridChange(int type)
{
	if (type == 0)
	{
		cout << "four" << endl;
		for (TileMap::size_type y = 0; y < tileMap.size(); y++)
		{
			for (TileRow::size_type x = 0; x < tileMap[y].size(); x++)
			{
				tileMap[y][x]->ClearNeighbors();
				tileMap[y][x]->SetPosition(sf::Vector2f(x * tileSize, y * tileSize));
				if (y > 0)
					tileMap[y][x]->SetNeighbor(tileMap[y - 1][x]);
				if (x > 0)
					tileMap[y][x]->SetNeighbor(tileMap[y][x - 1]);
				if (x < tileMap[y].size() - 1)
					tileMap[y][x]->SetNeighbor(tileMap[y][x + 1]);
				if (y < tileMap.size() - 1)
					tileMap[y][x]->SetNeighbor(tileMap[y + 1][x]);
			}
		}
	}
	if (type == 1)
	{
		cout << "eight" << endl;
		for (TileMap::size_type y = 0; y < tileMap.size(); y++)
		{
			for (TileRow::size_type x = 0; x < tileMap[y].size(); x++)
			{
				tileMap[y][x]->ClearNeighbors();
				tileMap[y][x]->SetPosition(sf::Vector2f(x * tileSize, y * tileSize));
				if (y > 0)
				{
					if (x > 0)
						tileMap[y][x]->SetNeighbor(tileMap[y - 1][x - 1]);
					tileMap[y][x]->SetNeighbor(tileMap[y - 1][x]);
					if (x < tileMap[y].size() - 1)
						tileMap[y][x]->SetNeighbor(tileMap[y - 1][x + 1]);
				}
				if (x > 0)
				{
					tileMap[y][x]->SetNeighbor(tileMap[y][x - 1]);
				}
				if (x < tileMap[y].size() - 1)
				{
					tileMap[y][x]->SetNeighbor(tileMap[y][x + 1]);
				}
				if (y < tileMap.size() - 1)
				{
					if (x > 0)
						tileMap[y][x]->SetNeighbor(tileMap[y + 1][x - 1]);
					tileMap[y][x]->SetNeighbor(tileMap[y + 1][x]);
					if (x < tileMap[y].size() - 1)
						tileMap[y][x]->SetNeighbor(tileMap[y + 1][x + 1]);
				}
			}
		}
	}
	if (type == 2)
	{
		cout << "hex" << endl;
		for (TileMap::size_type y = 0; y < tileMap.size(); y++)
		{
			for (TileRow::size_type x = 0; x < tileMap[y].size(); x++)
			{
				tileMap[y][x]->ClearNeighbors();
				tileMap[y][x]->SetPosition(sf::Vector2f(x * tileSize, y * tileSize + 16 * (x % 2)));
				if (x % 2 == 0)
				{
					if (y > 0)
					{
						if (x > 0)
							tileMap[y][x]->SetNeighbor(tileMap[y - 1][x - 1]);
						tileMap[y][x]->SetNeighbor(tileMap[y - 1][x]);
						if (x < tileMap[y].size() - 1)
							tileMap[y][x]->SetNeighbor(tileMap[y - 1][x + 1]);
					}
					if (x > 0)
					{
						tileMap[y][x]->SetNeighbor(tileMap[y][x - 1]);
					}
					if (y < tileMap.size() - 1)
					{
						tileMap[y][x]->SetNeighbor(tileMap[y + 1][x]);
					}
					if (x < tileMap[y].size() - 1)
					{
						tileMap[y][x]->SetNeighbor(tileMap[y][x + 1]);
					}
				}
				else
				{
					if (x > 0)
					{
						tileMap[y][x]->SetNeighbor(tileMap[y][x - 1]);
					}
					if (y > 0)
					{
						tileMap[y][x]->SetNeighbor(tileMap[y - 1][x]);
					}
					if (x < tileMap[y].size() - 1)
					{
						tileMap[y][x]->SetNeighbor(tileMap[y][x + 1]);
					}
					if (y < tileMap.size() - 1)
					{
						if (x > 0)
							tileMap[y][x]->SetNeighbor(tileMap[y + 1][x - 1]);
						tileMap[y][x]->SetNeighbor(tileMap[y + 1][x]);
						if (x < tileMap[y].size() - 1)
							tileMap[y][x]->SetNeighbor(tileMap[y + 1][x + 1]);
					}
				}
			}
		}
	}
}
