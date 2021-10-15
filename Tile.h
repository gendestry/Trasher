#pragma once
#include "Entity.h"

enum class TileType {
	INVALID,
	WATER,
	GRASS,
	SAND,
	MOUNTAIN
};

class Tile : public Entity {
	TileType m_Type;
public:
	Tile(glm::vec3 position, TileType type);
	~Tile();

	void update() override;

	TileType getType();
};

