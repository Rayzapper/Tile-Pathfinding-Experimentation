#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>

class Entity
{
public:
	Entity();
	~Entity();
	static void Initialize(sf::RenderWindow *mainWindow);
	void Update();
	void Render();
};

#endif