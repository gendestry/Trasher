#pragma once
#include "Entity.h"
#include <chrono>

class Player : public Entity {
	bool m_OnShip;
	bool m_CanToggleShip = true;
	int m_ToggleTime = 500;
	int m_ShipID = -1;

	int m_Score = 0;

	std::chrono::steady_clock::time_point start;
private:
	bool tileCollision() override;
	bool entityCollision() override;
	void updateModelMatrix();
public:
	Player(glm::vec3 position, int shipID);
	~Player();

	void move(glm::vec2 dir);

	void update() override;
	void render() override;

	void toggleShip();

	bool isOnShip();
	int getShipID();
	int getScore();
};

