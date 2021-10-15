#pragma once
#include <vector>
#include "Entity.h"
#include "Player.h"
#include "Tile.h"

class Manager {
	std::vector<Entity*> m_Entities;
	std::vector<Tile*> m_Tiles;
	Player* m_Player = nullptr;
public:
	Manager();
	~Manager();

	void addEntity(Entity* e);
	void flushEntities();

	void addTile(Tile* t);
	void flushTiles();

	void update(float deltaTime);
	void render();

	bool isLevelComplete();

	Player* getPlayer();
	Player** getPlayerPtr();

	const std::vector<Entity*>& getEntities();
	const std::vector<Tile*>& getTiles();

	std::vector<Entity*> getEntitiesWithType(EntityType type);

	std::vector<Tile*> getNeighbourTiles(Tile* t);
	Tile* getTileAt(float x, float y);
	TileType tileTypeAt(float x, float y);
};

