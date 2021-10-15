#pragma once
#include "Entity.h"

class Enemy : public Entity {
	
public:
	Enemy(glm::vec3 position, int islandID);
	~Enemy();

	void update() override;
};

