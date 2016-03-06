#include "Game.h"

static sf::RenderWindow *window;
static sf::View *mainView, *uiView;
static const int screenWidth = 1000, screenHeight = 656, tileSize = 32, mapSizeX = 20, mapSizeY = 20;
static bool focus = true, mouseReleased = true;

typedef vector<Tile*> TileRow;
typedef vector<TileRow> TileMap;
static TileMap tileMap;
typedef vector<Entity*> EntityVector;
static EntityVector entityVector;

static sf::Vector2i mousePosition;

sf::Font font;

sf::RectangleShape hexButton, fourButton, eightButton, stepButton;
sf::Text hexButtonText, fourButtonText, eightButtonText, stepButtonText;
sf::IntRect hexButtonBox, fourButtonBox, eightButtonBox, stepButtonBox;

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
					tileMap[y][x]->SetType(0);
				}
				else if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
				{
					tileMap[y][x]->SetType(1);
				}
			}
			tileMap[y][x]->Update();
		}
	}
	
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && mouseReleased)
	{
		mouseReleased = false;
		if (fourButtonBox.contains(mousePosition))
		{
			GridChange(0);
		}
		else if (eightButtonBox.contains(mousePosition))
		{
			GridChange(1);
		}
		else if (hexButtonBox.contains(mousePosition))
		{
			GridChange(2);
		}
		else if (stepButtonBox.contains(mousePosition))
		{
			
		}
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
	window->draw(fourButton);
	window->draw(fourButtonText);
	window->draw(eightButton);
	window->draw(eightButtonText);
	window->draw(hexButton);
	window->draw(hexButtonText);
	window->draw(stepButton);
	window->draw(stepButtonText);
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
	fourButton.setFillColor(sf::Color::Red);
	fourButton.setPosition(sf::Vector2f(screenWidth - 150, 40));
	fourButton.setSize(sf::Vector2f(100, 50));
	fourButtonText.setString("Four");
	fourButtonText.setFont(font);
	fourButtonText.setPosition(fourButton.getPosition());
	fourButtonBox.width = 100;
	fourButtonBox.height = 50;
	fourButtonBox.left = screenWidth - 150;
	fourButtonBox.top = 40;

	eightButton.setFillColor(sf::Color::Red);
	eightButton.setPosition(sf::Vector2f(screenWidth - 150, 100));
	eightButton.setSize(sf::Vector2f(100, 50));
	eightButtonText.setString("Eight");
	eightButtonText.setFont(font);
	eightButtonText.setPosition(eightButton.getPosition());
	eightButtonBox.width = 100;
	eightButtonBox.height = 50;
	eightButtonBox.left = screenWidth - 150;
	eightButtonBox.top = 100;

	hexButton.setFillColor(sf::Color::Red);
	hexButton.setPosition(sf::Vector2f(screenWidth - 150, 160));
	hexButton.setSize(sf::Vector2f(100, 50));
	hexButtonText.setString("Hex");
	hexButtonText.setFont(font);
	hexButtonText.setPosition(hexButton.getPosition());
	hexButtonBox.width = 100;
	hexButtonBox.height = 50;
	hexButtonBox.left = screenWidth - 150;
	hexButtonBox.top = 160;

	stepButton.setFillColor(sf::Color::Blue);
	stepButton.setPosition(sf::Vector2f(screenWidth - 300, 40));
	stepButton.setSize(sf::Vector2f(100, 50));
	stepButtonText.setString("Step");
	stepButtonText.setFont(font);
	stepButtonText.setPosition(stepButton.getPosition());
	stepButtonBox.width = 100;
	stepButtonBox.height = 50;
	stepButtonBox.left = screenWidth - 300;
	stepButtonBox.top = 40;
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
