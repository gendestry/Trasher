#include "Manager.h"
#include <algorithm>

Manager::Manager() {}

Manager::~Manager() {
	flushEntities();
	flushTiles();
}

void Manager::addEntity(Entity* entity) {
	if (entity->getEntityType() == EntityType::TILE)
		m_Tiles.push_back((Tile*)entity);
	else {
		m_Entities.push_back(entity);
		if (entity->getEntityType() == EntityType::PLAYER && m_Player == nullptr)
			m_Player = (Player*)entity;
	}
}

void Manager::flushEntities() {
	for (auto& e : m_Entities)
		delete e;

	m_Entities.clear();
	m_Player = nullptr;
}

void Manager::addTile(Tile* t) {
	m_Tiles.push_back(t);
}

void Manager::flushTiles() {
	for (auto& t : m_Tiles)
		delete t;

	m_Tiles.clear();
}

void Manager::update(float deltaTime) {
	for (int i = 0; i < m_Entities.size(); i++) {
		if (!m_Entities[i]->isActive()) {
			delete m_Entities[i];
			m_Entities.erase(m_Entities.begin() + i);
		}
	}
	
	for (auto& e : m_Entities)
		e->update();
}

void Manager::render() {
	for (auto& t : m_Tiles)
		t->render();
	
	for (auto& e : m_Entities)
		e->render();
}

bool Manager::isLevelComplete() {
	int num = 0;

	for (auto& e : m_Entities) {
		if ((e->getEntityType() == EntityType::ENEMY || e->getEntityType() == EntityType::GARBAGE) && e->isActive())
			num++;
	}	

	if (num == 0) return true;
	else return false;
}

Player* Manager::getPlayer() {
	return m_Player;
}

Player** Manager::getPlayerPtr() {
	return &m_Player;
}

const std::vector<Entity*>& Manager::getEntities() {
	return m_Entities;
}

const std::vector<Tile*>& Manager::getTiles() {
	return m_Tiles;
}

std::vector<Entity*> Manager::getEntitiesWithType(EntityType type) {
	if (type == EntityType::TILE)
		return std::vector<Entity*>(m_Tiles.begin(), m_Tiles.end());

	std::vector<Entity*> ret;

	for (auto& e : m_Entities)
		if (e->getEntityType() == type)
			ret.push_back(e);

	return ret;
}

std::vector<Tile*> Manager::getNeighbourTiles(Tile* t) {
	std::vector<Tile*> ret;

	auto& tpos = t->getPosition();
	auto tsize = t->getSize();

	Tile* temp = getTileAt(tpos.x - tsize, tpos.y);
	if (temp != nullptr)
		ret.push_back(temp);

	temp = getTileAt(tpos.x + tsize, tpos.y);
	if (temp != nullptr)
		ret.push_back(temp);

	temp = getTileAt(tpos.x, tpos.y - tsize);
	if (temp != nullptr)
		ret.push_back(temp);

	temp = getTileAt(tpos.x, tpos.y + tsize);
	if (temp != nullptr)
		ret.push_back(temp);

	return ret;
}

Tile* Manager::getTileAt(float x, float y) {
	for (auto& t : m_Tiles) {
		auto& pos = t->getPosition();

		if (pos.x == x && pos.y == y)
			return t;
	}

	return nullptr;
}

TileType Manager::tileTypeAt(float x, float y) {
	for (auto& t : m_Tiles) {
		auto& pos = t->getPosition();

		if (pos.x == x && pos.y == y)
			return t->getType();
	}

	return TileType::INVALID;
}