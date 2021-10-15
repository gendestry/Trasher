#include "Tile.h"
#include "Game.h"

Tile::Tile(glm::vec3 position, TileType type) : Entity(position), m_Type(type) {
	m_EntityType = EntityType::TILE;

	m_Shader = m_GamePtr->getResourceManager().getShader("tile");
	
	switch (type) {
	case TileType::WATER:
		m_Sprite = m_GamePtr->getResourceManager().getSprite("tile_water");
		break;
	case TileType::GRASS:
		m_Sprite = m_GamePtr->getResourceManager().getSprite("tile_grass");
		break;
	case TileType::SAND:
		m_Sprite = m_GamePtr->getResourceManager().getSprite("tile_sand");
		break;
	case TileType::MOUNTAIN:
		m_Sprite = m_GamePtr->getResourceManager().getSprite("tile_mountain");
		break;
	default:
		m_Sprite = m_GamePtr->getResourceManager().getSprite("tile_grass");
		break;
	};
}

Tile::~Tile() {}

void Tile::update() {}

TileType Tile::getType() {
	return m_Type;
}
