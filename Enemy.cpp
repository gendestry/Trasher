#include "Enemy.h"
#include "Game.h"

Enemy::Enemy(glm::vec3 position, int islandID) : Entity(position, islandID) {
	m_EntityType = EntityType::ENEMY;

	m_Margin = 1.0f;
	m_MovementSpeed = 2.0f;
	m_Sprite = m_GamePtr->getResourceManager().getSprite("enemy");
}

Enemy::~Enemy() {}

void Enemy::update() {
	if(m_ManagerPtr->getPlayer()->getIslandID() == m_IslandID)
		moveTowardsPlayer();
	else {
		moveRandomly();
	}

	updateModelMatrix();
}