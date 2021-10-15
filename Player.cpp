#include "Player.h"
#include "Game.h"
#include <vector>
#include <iostream>
#include "Tile.h"
#include "glm/gtc/matrix_transform.hpp"

typedef std::chrono::high_resolution_clock Clock;

Player::Player(glm::vec3 position, int shipID) : Entity(position) {
	m_EntityType = EntityType::PLAYER;
	m_ShipID = shipID;
	m_OnShip = shipID != -1;
	m_MovementSpeed = 2.0f;
	updateModelMatrix();

	m_Sprite = m_GamePtr->getResourceManager().getSprite("player");
	m_Shader = m_GamePtr->getResourceManager().getShader("player");
}

Player::~Player() {}

void Player::move(glm::vec2 dir) {
	dir *= m_MovementSpeed;

	auto oldpos = m_Position;

	m_Position.x += dir.x;
	if (collide()) m_Position.x = oldpos.x;
	m_Position.y += dir.y;
	if (collide()) m_Position.y = oldpos.y;
	updateModelMatrix();
}

void Player::update() {
	entityCollision();

	if (!m_CanToggleShip) {	
		auto now = Clock::now();

		if (std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count() > m_ToggleTime) {
			m_CanToggleShip = true;
		}
	}
}

void Player::render() {
	m_Shader->use();
	m_Shader->setMat4("uModel", m_Model);
	m_Sprite->render();
}

void Player::toggleShip() {
	if (!m_CanToggleShip) 
		return;

	// if on land
	if (!m_OnShip) {
		std::vector<Entity*> ships = m_ManagerPtr->getEntitiesWithType(EntityType::SHIP);

		for (auto& s : ships) {
			float dist = distanceBetween(s);
			if (dist < m_Size + m_Size / 2) {
				m_IslandID = -1;
				m_OnShip = true;
				m_ShipID = s->getID();
				m_Position.x = s->getPosition().x;
				m_Position.y = s->getPosition().y;
				break;
			}
		}
	}
	else {
		std::vector<Entity*> tiles = m_ManagerPtr->getEntitiesWithType(EntityType::TILE);

		float shortestDistance = m_Size * 2;
		bool canEject = false;

		glm::vec3 newPosition;

		for (auto& e : tiles) {
			Tile* t = (Tile*)e;

			if (t->getType() == TileType::WATER || t->getType() == TileType::MOUNTAIN)
				continue;
			
			float dist = distanceBetween(t);
			if (dist < m_Size + m_Size / 2 && shortestDistance > dist) {
				canEject = true;
				shortestDistance = dist;
				newPosition = t->getPosition();
			}
		}

		if (canEject) {
			m_OnShip = false;
			m_ShipID = -1;
			m_IslandID = m_GamePtr->getLevel()->getIslandIndex(newPosition.x, newPosition.y);
			m_Position.x = newPosition.x;
			m_Position.y = newPosition.y;
		}
	}

	updateModelMatrix();
	m_CanToggleShip = false;
	start = Clock::now();
}

bool Player::isOnShip() {
	return m_OnShip;
}

int Player::getShipID() {
	return m_ShipID;
}

int Player::getScore() {
	return m_Score;
}

bool Player::tileCollision() {
	const std::vector<Tile*>& tiles = m_ManagerPtr->getTiles();

	for (int i = 0; i < tiles.size(); i++) {
		bool scond = (m_OnShip && tiles[i]->getType() == TileType::WATER);
		bool pcond = (!m_OnShip && (tiles[i]->getType() == TileType::GRASS || tiles[i]->getType() == TileType::SAND));

		if (scond || pcond) continue;

		if (intersects(tiles[i]))
			return true;
	}

	return false;
}

// add check for 2 or more enemies
bool Player::entityCollision() {
	const std::vector<Entity*>& entities = m_ManagerPtr->getEntities();

	int enemiesHit = 0;
	for (int i = 0; i < entities.size(); i++) {
		EntityType type = entities[i]->getEntityType();
		if (type == EntityType::SHIP)
			continue;

		if (intersects(entities[i]) && entities[i]->isActive()) {
			entities[i]->destroy();
			if (type == EntityType::ENEMY) {
				enemiesHit++;
				m_Score += 10;
			}
			else if (type == EntityType::GARBAGE)
				m_Score += 5;

			if(m_ManagerPtr->isLevelComplete())
				std::cout << "You won, your score: " << m_Score << std::endl;
			//return true;
		}
	}

	if (enemiesHit > 1) {
		std::cout << "You die, your score: " << m_Score << std::endl;
	}

	return false;
}

void Player::updateModelMatrix() {
	m_Model = glm::mat4(1.0);
	if (m_OnShip) {
		float scalar = 1.4f;
		float ratio = m_Size / scalar;

		auto pos = m_Position;
		pos.x += (m_Size - ratio) / 2;
		pos.y += (m_Size - ratio) / 2;

		m_Model = glm::translate(m_Model, pos);
		m_Model = glm::scale(m_Model, { ratio, ratio, 1.0f });
	}
	else {
		m_Model = glm::translate(m_Model, m_Position);
		m_Model = glm::scale(m_Model, { m_Size, m_Size, 1.0f });
	}
}