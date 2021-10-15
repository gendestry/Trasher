#pragma once
#include "Entity.h"

class Garbage : public Entity {
	bool tileCollision() override;
public:
	Garbage(glm::vec3 position);
	~Garbage();

	// void update() override {}
};

