#include "Garbage.h"
#include "Game.h"

Garbage::Garbage(glm::vec3 position) : Entity(position) {
	m_EntityType = EntityType::GARBAGE;
	m_MovementSpeed = 2.0f;
	updateModelMatrix();

	m_Sprite = m_GamePtr->getResourceManager().getSprite("garbage");
}

Garbage::~Garbage() {}

bool Garbage::tileCollision() {
	const std::vector<Tile*>& tiles = m_ManagerPtr->getTiles();

	for (int i = 0; i < tiles.size(); i++) {
		if (tiles[i]->getType() == TileType::WATER) continue;

		if (intersects(tiles[i]))
			return true;
	}
	return false;
}
