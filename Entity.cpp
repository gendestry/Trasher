#include "Entity.h"
#include "Game.h"
#include <stdlib.h>
#include "Manager.h"
#include "glm/gtc/matrix_transform.hpp"

Game* Entity::m_GamePtr = nullptr;
Manager* Entity::m_ManagerPtr = nullptr;
int Entity::m_IDCount = 0;

void Entity::setGamePointer(Game* game) {
	m_GamePtr = game;
}

Entity::Entity(glm::vec3 position) : m_Position(position) {
	if (m_ManagerPtr == nullptr)
		m_ManagerPtr = &m_GamePtr->getManager();

	m_ID = m_IDCount++;

	m_Shader = m_GamePtr->getResourceManager().getShader("entity");
	m_Size = m_GamePtr->getEntitySize();

	updateModelMatrix();
}

Entity::Entity(glm::vec3 position, int islandID) : m_Position(position), m_IslandID(islandID) {
	if (m_ManagerPtr == nullptr)
		m_ManagerPtr = &m_GamePtr->getManager();

	m_ID = m_IDCount++;

	m_Shader = m_GamePtr->getResourceManager().getShader("entity");
	m_Size = m_GamePtr->getEntitySize();

	updateModelMatrix();
}

Entity::~Entity() {}

bool Entity::isActive() {
	return m_Active;
}

void Entity::destroy() {
	m_Active = false;
}

bool Entity::intersects(Entity* e) {
	if (m_ID == e->getID()) // self check
		return false;

	float margin = m_Margin + e->getMargin();
	float esize = e->getSize();

	auto& epos = e->getPosition();
	if (m_Position.x + m_Size - margin > epos.x && m_Position.x + margin < epos.x + esize &&
		m_Position.y + m_Size - margin > epos.y && m_Position.y + margin < epos.y + esize) {
		return true;
	}
	return false;
}

/*bool Entity::intersects(glm::vec2 pos, glm::vec2 size) {
	float margin = m_Margin;

	if (m_Position.x + m_Size - margin > pos.x && m_Position.x + margin < pos.x + m_Size &&
		m_Position.y + m_Size - margin > pos.y && m_Position.y + margin < pos.y + m_Size) {
		return true;
	}
	return false;
}*/

float Entity::distanceBetween(Entity* e) {
	return glm::distance(m_Position, e->getPosition());
}

void Entity::update() {
	moveRandomly();

	updateModelMatrix();
}

void Entity::render() {
	m_Shader->use();

	/*const glm::vec3& pPos = m_ManagerPtr->getPlayer()->getPosition();
	if(m_ManagerPtr->getPlayer() != nullptr)
		m_Shader->setVec3("uPlayerPos", { pPos.x + m_Size / 2, pPos.y + m_Size / 2, pPos.z });*/
	
	m_Shader->setMat4("uModel", m_Model);
	m_Sprite->render();
}

// Getters
int Entity::getID() const {
	return m_ID;
}

int Entity::getIslandID() const {
	return m_IslandID;
}

const glm::vec3& Entity::getPosition() const {
	return m_Position;
}

float Entity::getSize() const {
	return m_Size;
}

float Entity::getMargin() const {
	return m_Margin;
}

float Entity::getMovementSpeed() const {
	return m_MovementSpeed;
}

EntityType Entity::getEntityType() {
	return m_EntityType;
}


// Protected; may cause seizures
void Entity::moveRandomly() {
	//int rad = (rand() % 360) * 3.141526 / 180;
	//m_Position.x += m_MovementSpeed * cos(rad);
	//m_Position.y += m_MovementSpeed * sin(rad);
	auto oldpos = m_Position;

	int dir = rand() % 4;

	switch (dir) {
	case 0:
		m_Position.x += m_MovementSpeed;
		break;
	case 1:
		m_Position.x -= m_MovementSpeed;
		break;
	case 2:
		m_Position.y += m_MovementSpeed;
		break;
	case 3:
		m_Position.y -= m_MovementSpeed;
		break;
	default:
		break;
	};

	if (collide())
		m_Position = oldpos;
}

void Entity::moveTowardsPlayer() {
	auto oldpos = m_Position;

	auto& ppos = m_ManagerPtr->getPlayer()->getPosition();
	auto dir = glm::normalize(ppos - m_Position);
	
	m_Position.x += dir.x * m_MovementSpeed;
	if (collide())
		m_Position.x = oldpos.x;

	m_Position.y += dir.y * m_MovementSpeed;
	if (collide())
		m_Position.y = oldpos.y;
}

bool Entity::pathFind() {
	// make a LOT better cuz this sucks !
	m_Path.clear();
	m_FoundPath = false;
	pathFindRecursive(glm::vec3(0.0f));

	return m_FoundPath;	
}

void Entity::pathFindRecursive(glm::vec3 pathPos) {
	/*auto player = m_ManagerPtr->getPlayer();

	if (player->isOnShip() || m_FoundPath)
		return;

	float x = pathPos.x, y = pathPos.y;
	float level = pathPos.z + 1.0f;

	std::vector<glm::vec3> temp;

	if (level == 1.0f) {
		x = (int)m_Position.x - ((int)m_Position.x % (int)m_Size);
		y = (int)m_Position.y - ((int)m_Position.y % (int)m_Size);
	}

	TileType type = m_ManagerPtr->tileTypeAt(x - 32.0f, y); // left
	if (type == TileType::GRASS || type == TileType::SAND)
		temp.emplace_back(x - 32.0f, y, level);

	type = m_ManagerPtr->tileTypeAt(x + 32.0f, y); // right
	if (type == TileType::GRASS || type == TileType::SAND)
		temp.emplace_back(x + 32.0f, y, level);

	type = m_ManagerPtr->tileTypeAt(x, y - 32.0f); // top
	if (type == TileType::GRASS || type == TileType::SAND)
		temp.emplace_back(x, y - 32.0f, level);

	type = m_ManagerPtr->tileTypeAt(x, y + 32.0f); // bottom
	if (type == TileType::GRASS || type == TileType::SAND)
		temp.emplace_back(x, y + 32.0f, level);

	for (auto& t : temp) {
		if (player->intersects(t)) {
			m_FoundPath = true;
			m_Path.push_back(t);
			break;
		}
	}
	
	if (!m_FoundPath) {
		for (auto& p : m_Path) {
			for (int i = 0; i < temp.size(); i++) {
				auto& tp = temp[i];
				if (p.x == tp.x && p.y == tp.y) {
					if (p.z <= tp.z)
						temp.erase(temp.begin() + i);
				}
			}
		}

		m_Path.insert(m_Path.end(), temp.begin(), temp.end());

		for (auto& t : temp)
			pathFindRecursive({ t.x, t.y, level++ });
	}*/

	/*}
	else {
		for (int i = 0; i < m_Path.size(); i++) {
			int x = m_Path[i].x;
			int y = m_Path[i].y;

			TileType type = m_ManagerPtr->tileTypeAt(x - 32.0f, y); // left
			if (type == TileType::GRASS || type == TileType::SAND)
				m_Path.emplace_back(x - 32.0f, y, level);

			type = m_ManagerPtr->tileTypeAt(x + 32.0f, y); // right
			if (type == TileType::GRASS || type == TileType::SAND)
				m_Path.emplace_back(x + 32.0f, y, level);

			type = m_ManagerPtr->tileTypeAt(x, y - 32.0f); // top
			if (type == TileType::GRASS || type == TileType::SAND)
				m_Path.emplace_back(x, y - 32.0f, level);

			type = m_ManagerPtr->tileTypeAt(x, y + 32.0f); // bottom
			if (type == TileType::GRASS || type == TileType::SAND)
				m_Path.emplace_back(x, y + 32.0f, level);
		}
	}*/
}

void Entity::updateModelMatrix() {
	m_Model = glm::mat4(1.0);
	m_Model = glm::translate(m_Model, m_Position);
	m_Model = glm::scale(m_Model, { m_Size, m_Size, 1.0f });
}


// Collision
bool Entity::collide() {
	if (m_Position.x + m_Size - m_Margin > m_GamePtr->getLevel()->getNumColumns() * m_Size)
		return true;
	else if (m_Position.x - m_Margin < 0.0f)
		return true;

	if (m_Position.y + m_Size - m_Margin > m_GamePtr->getLevel()->getNumRows() * m_Size)
		return true;
	else if (m_Position.y - m_Margin < 0.0f)
		return true;
	/*if (m_Position.x + m_Size - m_Margin > m_GamePtr->getWidth())
		return true;
	else if (m_Position.x - m_Margin < 0.0f)
		return true;
	if (m_Position.y + m_Size - m_Margin > m_GamePtr->getHeight())
		return true;
	else if (m_Position.y - m_Margin < 0.0f)
		return true;*/

	return tileCollision() || entityCollision();
}

bool Entity::tileCollision() {
	const std::vector<Tile*>& tiles = m_ManagerPtr->getTiles();

	for (int i = 0; i < tiles.size(); i++) {
		if (tiles[i]->getType() == TileType::GRASS || tiles[i]->getType() == TileType::SAND) continue;

		if (intersects(tiles[i]))
			return true;
	}
	return false;
}

bool Entity::entityCollision() {
	return false;
}