#include "Ship.h"
#include "Game.h"


Ship::Ship(glm::vec3 position) : Entity(position) {
	m_EntityType = EntityType::SHIP;
	m_PlayerPtr = m_ManagerPtr->getPlayerPtr();
	m_Sprite = m_GamePtr->getResourceManager().getSprite("ship");
}

Ship::~Ship() {}

void Ship::update() {
	if ((*m_PlayerPtr) != nullptr && (*m_PlayerPtr)->isOnShip() && (*m_PlayerPtr)->getShipID() == m_ID) {
		glm::vec3 ppos = (*m_PlayerPtr)->getPosition();

		m_Position.x = ppos.x;
		m_Position.y = ppos.y;

		updateModelMatrix();
	}
}

bool Ship::tileCollision() {
	const std::vector<Tile*>& tiles = m_ManagerPtr->getTiles();

	for (int i = 0; i < tiles.size(); i++) {
		if (tiles[i]->getType() == TileType::WATER) continue;

		if (intersects(tiles[i]))
			return true;
	}
	return false;
}