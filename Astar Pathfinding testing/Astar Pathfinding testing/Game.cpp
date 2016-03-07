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

enum GridType
{
	FOUR,
	EIGHT,
	HEX
};

GridType gridType = FOUR;

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
			for (TileMap::size_type y = 0; y < tileMap.size(); y++)
				for (TileRow::size_type x = 0; x < tileMap[y].size(); x++)
					tileMap[y][x]->SetPath(false);
			vector<Tile*> path = Pathfind(startTile, targetTile);
			for (vector<Tile*>::size_type i = 0; i < path.size(); i++)
			{
				path[i]->SetPath(true);
				//cout << path[i] << endl;
			}
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
		gridType = FOUR;
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
		gridType = EIGHT;
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
	//if (type == 2)
	//{
	//	cout << "hex" << endl;
	//	gridType = HEX;
	//	for (TileMap::size_type y = 0; y < tileMap.size(); y++)
	//	{
	//		for (TileRow::size_type x = 0; x < tileMap[y].size(); x++)
	//		{
	//			tileMap[y][x]->ClearNeighbors();
	//			tileMap[y][x]->SetPosition(sf::Vector2f(x * tileSize, y * tileSize + 16 * (x % 2)));
	//			if (x % 2 == 0)
	//			{
	//				if (y > 0)
	//				{
	//					if (x > 0)
	//						tileMap[y][x]->SetNeighbor(tileMap[y - 1][x - 1]);
	//					tileMap[y][x]->SetNeighbor(tileMap[y - 1][x]);
	//					if (x < tileMap[y].size() - 1)
	//						tileMap[y][x]->SetNeighbor(tileMap[y - 1][x + 1]);
	//				}
	//				if (x > 0)
	//				{
	//					tileMap[y][x]->SetNeighbor(tileMap[y][x - 1]);
	//				}
	//				if (y < tileMap.size() - 1)
	//				{
	//					tileMap[y][x]->SetNeighbor(tileMap[y + 1][x]);
	//				}
	//				if (x < tileMap[y].size() - 1)
	//				{
	//					tileMap[y][x]->SetNeighbor(tileMap[y][x + 1]);
	//				}
	//			}
	//			else
	//			{
	//				if (x > 0)
	//				{
	//					tileMap[y][x]->SetNeighbor(tileMap[y][x - 1]);
	//				}
	//				if (y > 0)
	//				{
	//					tileMap[y][x]->SetNeighbor(tileMap[y - 1][x]);
	//				}
	//				if (x < tileMap[y].size() - 1)
	//				{
	//					tileMap[y][x]->SetNeighbor(tileMap[y][x + 1]);
	//				}
	//				if (y < tileMap.size() - 1)
	//				{
	//					if (x > 0)
	//						tileMap[y][x]->SetNeighbor(tileMap[y + 1][x - 1]);
	//					tileMap[y][x]->SetNeighbor(tileMap[y + 1][x]);
	//					if (x < tileMap[y].size() - 1)
	//						tileMap[y][x]->SetNeighbor(tileMap[y + 1][x + 1]);
	//				}
	//			}

	//			//////////////////////////////////////////////////////////////////////
	//			/*int xDifference, yDifference, distanceCost = 0;
	//			xDifference = startTile->GetGridPosition().x - tileMap[y][x]->GetGridPosition().x;
	//			xDifference = abs(xDifference);
	//			yDifference = startTile->GetGridPosition().y - tileMap[y][x]->GetGridPosition().y;
	//			yDifference = abs(yDifference);
	//			if (startTile->GetGridPosition().x % 2 == 0)
	//			{

	//			}
	//			tileMap[y][x]->testInt = distanceCost;*/
	//			//////////////////////////////////////////////////////////////////////
	//		}
	//	}
	//}
}

vector<Tile*> Game::Pathfind(Tile *start, Tile *target)
{
	vector<Tile*>
		open,			//Nodes whose F cost has been calculated.
		closed;			//Nodes that have been evaluated.
	start->SetPathValues(0, GetDistanceCost(start, target));
	open.push_back(start);
	bool loop = true;

	Tile *current = start;
	while (loop)
	{
		current = open[0];
		for (vector<Tile*>::size_type i = 0; i < open.size(); i++)
		{
			if (open[i]->GetPathValues().fCost < current->GetPathValues().fCost)
				current = open[i];
		}

		open.erase(find(open.begin(), open.end(), current));

		closed.push_back(current);

		if (current == target)
		{
			loop = false;
			break;
		}

		for each (Tile* neighbor in current->GetNeighbors())
		{
			int newPathLength = CheckPathLength(current, 0);
			if (gridType == EIGHT && current->GetGridPosition().x != neighbor->GetGridPosition().x
				&& current->GetGridPosition().y != neighbor->GetGridPosition().y)
				newPathLength += 14;
			else
				newPathLength += 10;

			if (neighbor->GetType() == 1 || find(closed.begin(), closed.end(), neighbor) != closed.end())
				; //Skip
			else if (newPathLength < CheckPathLength(neighbor, 0) || find(open.begin(), open.end(), neighbor) == open.end())
			{
				neighbor->SetPathValues(newPathLength, GetDistanceCost(neighbor, target));
				neighbor->SetPathParent(current);

				if (find(open.begin(), open.end(), neighbor) == open.end())
				{
					open.push_back(neighbor);
				}
			}
		}
	}

	vector<Tile*> path;
	path = current->GetPath(path);
	cout << path.size();

	for each (Tile* t in open)
	{
		t->SetPathParent(nullptr);
		t->SetPathValues(0, 0);
	}
	for each (Tile* t in closed)
	{
		t->SetPathParent(nullptr);
		t->SetPathValues(0, 0);
	}

	return path;
}

int Game::GetDistanceCost(Tile *tile1, Tile *tile2)
{
	int distanceCost, tile1X, tile1Y, tile2X, tile2Y;
	tile1X = tile1->GetGridPosition().x;
	tile1Y = tile1->GetGridPosition().y;
	tile2X = tile2->GetGridPosition().x;
	tile2Y = tile2->GetGridPosition().y;
	if (gridType == FOUR)
	{
		int xCost, yCost;
		xCost = tile1X - tile2X;
		xCost = abs(xCost);
		yCost = tile1Y - tile2Y;
		yCost = abs(yCost);
		distanceCost = (xCost + yCost) * 10;
	}
	else if (gridType == EIGHT)
	{
		int xCost, yCost, diaCost = 0;
		xCost = tile1X - tile2X;
		xCost = abs(xCost);
		yCost = tile1Y - tile2Y;
		yCost = abs(yCost);
		while (xCost > 0 && yCost > 0)
		{
			xCost--;
			yCost--;
			diaCost++;
		}
		distanceCost = (xCost + yCost) * 10 + diaCost * 14;
	}
	else if (gridType == HEX)
	{
		int xDifference, yDifference;
		xDifference = tile1X - tile2X;
		xDifference = abs(xDifference);
		yDifference = tile1Y - tile2Y;
		yDifference = abs(yDifference);
		//I AM TESTING THIS ELSEWHERE
	}
	return distanceCost;
}

int Game::CheckPathLength(Tile *tile, int oldLength)
{
	int length = oldLength;
	Tile *parent = tile->GetPathParent();
	if (parent != nullptr)
	{
		if (gridType == EIGHT && tile->GetGridPosition().x != parent->GetGridPosition().x
			&& tile->GetGridPosition().y != parent->GetGridPosition().y)
			length += 14;
		else
			length += 10;
		length = CheckPathLength(parent, length);
	}
	return length;
}
