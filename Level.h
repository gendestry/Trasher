#pragma once
#include <string>
#include <vector>
#include "Manager.h"
#include "Tile.h"

class Level {
	struct SaveTile {
		float x, y;
		int type;
	};

	struct SaveEntity {
		int id;
		float x, y;
		int type;
		//int islandID;
	};

	struct SaveScore {
		char name[30];
		int score;
	};

	const float TILE_Z_INDEX   = -0.9f;
	const float PLAYER_Z_INDEX = 0.5f;
	const float SHIP_Z_INDEX   = 0.49f;

	std::string m_Path;
	std::vector<std::vector<Tile*>> m_Islands;

	Manager* m_Manager;

	float m_Size = 32.0f;
	int m_Columns, m_Rows;

	void floodFillIsland(Tile* start);
	void mapIslands();
	void spawnPlayer();
	void spawnEntities();
public:
	Level(Manager* manager);
	~Level();

	void loadFromFile(std::string path);
	void loadFromSave();
	void saveGame();
	void saveScoreboard();
	void loadScoreboard();

	int getNumColumns();
	int getNumRows();
	int getIslandIndex(float x, float y);
	float getEntitySize() const;
};

