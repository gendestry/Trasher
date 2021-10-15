#pragma once
#include "Entity.h"
#include "Player.h"

class Ship : public Entity {
	Player** m_PlayerPtr;
	bool tileCollision() override;
public:
	Ship(glm::vec3 position);
	~Ship();

	void update() override;
};

