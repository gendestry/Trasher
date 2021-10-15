#include "Level.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <time.h>
#include <stdlib.h>
#include <algorithm>
#include "Tile.h"
#include "Player.h"
#include "Ship.h"
#include "Enemy.h"
#include "Garbage.h"


Level::Level(Manager* manager) : m_Manager(manager) {
	srand(time(0));
}

Level::~Level() {}

void Level::loadFromFile(std::string path) {
	m_Manager->flushTiles();
	m_Manager->flushEntities();

	m_Path = path;

	std::ifstream file;
	file.open(path);

	if (!file.is_open()) {
		std::cerr << "[LEVEL] File: " << path << " could not be opened!" << std::endl;
		return;
	}

	std::stringstream fileData;
	fileData << file.rdbuf();

	file.close();

	std::string data = fileData.str();
	float x = 0.0f, y = 0.0f;
	char c;

	for (int i = 0; i < data.length(); i++) {
		c = data.at(i);

		if (c == '0') {
			m_Manager->addTile(new Tile({ x, y, TILE_Z_INDEX }, TileType::WATER));
		}
		else if (c == '1') {
			m_Manager->addTile(new Tile({ x, y, TILE_Z_INDEX }, TileType::GRASS));
		}
		else if (c == '2') {
			m_Manager->addTile(new Tile({ x, y, TILE_Z_INDEX }, TileType::SAND));
		}
		else if (c == '3') {
			m_Manager->addTile(new Tile({ x, y, TILE_Z_INDEX }, TileType::MOUNTAIN));
		}

		x += m_Size;

		if (c == '\n') {
			x = 0.0f;
			y += m_Size;
		}
	}

	m_Columns = x / m_Size;
	m_Rows = (y / m_Size) + 1;

	// load entities
	mapIslands();
	spawnEntities();
	spawnPlayer();
}

void Level::loadFromSave() {
	m_Manager->flushTiles();
	m_Manager->flushEntities();

	m_Path = "quicksave.bin";

	std::ifstream savefile("quicksave.bin", std::ios::binary);
	int size = 0;
	
	// load tiles
	if(savefile.is_open()) {
		savefile.read((char*)&size, sizeof(int));

		SaveTile st;
		for (int i = 0; i < size; i++) {
			savefile.read((char*)&st, sizeof(SaveTile));
			m_Manager->addTile(new Tile({st.x, st.y, TILE_Z_INDEX}, (TileType)st.type));

			if (st.x / m_Size > m_Columns)
				m_Columns = st.x / m_Size;
			if (st.y / m_Size > m_Rows)
				m_Rows = st.y / m_Size;
		}

		mapIslands();

		// load entities
		/*savefile.read((char*)&size, sizeof(int));

		SaveEntity se;
		for (int i = 0; i < size; i++) {
			savefile.read((char*)&se, sizeof(SaveTile));
			std::cout << se.x << ", " << se.y << " - " << se.type << std::endl;
		}*/

		m_Manager->addEntity(new Enemy({ 96.0f, 96.0f, 0.001f }, 1));

		// load player

		// temp code for loading player
		Ship* ship = new Ship({ 32.0f, 32.0f, SHIP_Z_INDEX });
		m_Manager->addEntity(ship);
		m_Manager->addEntity(new Player({ 32.0f, 32.0f, PLAYER_Z_INDEX }, ship->getID()));
		
		savefile.close();
	}
	else {
		std::cout << "Save file not found!" << std::endl;
		loadFromFile("levels/level_01.lvl");
	}
}

void Level::saveGame() {
	std::ofstream savefile("quicksave.bin", std::ios::binary);
	auto& tiles = m_Manager->getTiles();

	// num of tiles
	int size = tiles.size();
	savefile.write((char*)&size, sizeof(int));

	SaveTile st;
	for (int i = 0; i < size; i++) {
		st.x = tiles[i]->getPosition().x;
		st.y = tiles[i]->getPosition().y;
		st.type = (int)tiles[i]->getType();
		savefile.write((char*)&st, sizeof(SaveTile));
	}

	// save entities


	// save player

	savefile.close();
	
}

void Level::saveScoreboard() {
	std::ofstream file("scoreboard.bin", std::ios::binary | std::ios::app);
	SaveScore ss;
	strcpy_s(ss.name, "Debug");
	ss.score = m_Manager->getPlayer()->getScore();
	file.write((char*)&ss, sizeof(SaveScore));
	file.close();
}

void Level::loadScoreboard() {
	std::vector<SaveScore> scores;
	std::ifstream file("scoreboard.bin", std::ios::binary);
	SaveScore ss;

	while (file.read((char*)&ss, sizeof(SaveScore))) {
		scores.push_back(ss);
	}

	file.close();

	std::sort(scores.begin(), scores.end(), [](SaveScore ss1, SaveScore ss2) {
		return ss1.score > ss2.score;
	});

	for(auto& s : scores)
		std::cout << s.name << ": " << s.score << std::endl;
}


int Level::getNumColumns() {
	return m_Columns;
}

int Level::getNumRows() {
	return m_Rows;
}

int Level::getIslandIndex(float x, float y) {
	for (int i = 0; i < m_Islands.size(); i++) {
		for (auto& tile : m_Islands[i]) {
			auto& pos = tile->getPosition();
			if (pos.x == x && pos.y == y)
				return i;
		}
	}

	return -1;
}

float Level::getEntitySize() const {
	return m_Size;
}


void Level::floodFillIsland(Tile* start) {
	std::vector<Tile*> island, temp, checked;
	island.push_back(start);

	for (int i = 0; i < island.size(); i++) {
		temp = m_Manager->getNeighbourTiles(island[i]);
		std::copy_if(temp.begin(), temp.end(), std::back_inserter(island), [&](Tile* tile) {
			if (tile->getType() == TileType::WATER || tile->getType() == TileType::MOUNTAIN)
				return false;

			for (auto& t : island)
				if (t->getPosition().x == tile->getPosition().x && t->getPosition().y == tile->getPosition().y)
					return false;

			return true;
		});
	}
	
	m_Islands.push_back(island);
}

void Level::mapIslands() {
	m_Islands.clear();

	auto& tiles = m_Manager->getTiles();

	for (int i = 0; i < tiles.size(); i++) {
		auto& tile = tiles[i];
		if (tile->getType() == TileType::WATER || tile->getType() == TileType::MOUNTAIN)
			continue;

		bool found = false;
		for (auto& vec : m_Islands) {
			if (found) break;
			for (auto& t : vec) {
				auto& tpos = t->getPosition();

				if (tpos.x == tile->getPosition().x && tpos.y == tile->getPosition().y) {
					found = true;
					break;
				}
			}
		}

		if (!found)
			floodFillIsland(tile);
	}

	std::cout << "Printing islands: \n";
	for (auto& vec : m_Islands) {
		std::cout << "New island: " << vec.size() << std::endl;
		for (auto& t : vec) {
			std::cout << t->getPosition().x << ", " << t->getPosition().y << " - " << (int)t->getType() << std::endl;
		}
		std::cout << std::endl;
	}

}

void Level::spawnEntities() {
	const std::vector<Tile*>& tiles = m_Manager->getTiles();

	float z = 0.001f;
	int numEnemies = 15;//rand() % 10 + 2;
	int numGarbage = rand() % 10 + 2;

	for (int i = 0; i < numEnemies; i++) {
		int islandNum = rand() % m_Islands.size();
		int islandTile = rand() % m_Islands[islandNum].size();

		auto& tilepos = m_Islands[islandNum][islandTile]->getPosition();

		m_Manager->addEntity(new Enemy({tilepos.x, tilepos.y, z}, islandNum));
		z += 0.001f;
	}

	for (int i = 0; i < numGarbage; i++) {
		float x = (rand() % m_Columns) * m_Size;
		float y = (rand() % m_Rows) * m_Size;

		bool canSpawn = false;

		while (!canSpawn) {
			TileType type = m_Manager->tileTypeAt(x, y);

			if (type == TileType::WATER)
				canSpawn = true;
			else {
				x = (rand() % m_Columns) * m_Size;
				y = (rand() % m_Rows) * m_Size;
			}
		}
		m_Manager->addEntity(new Garbage({ x, y, z }));
		z += 0.001f;
	}
}

void Level::spawnPlayer() {
	const std::vector<Tile*>& tiles = m_Manager->getTiles();

	float x = (rand() % m_Columns) * m_Size;
	float y = (rand() % m_Rows) * m_Size;

	bool canSpawn = false;

	while (!canSpawn) {
		TileType type = m_Manager->tileTypeAt(x, y);

		if (type == TileType::WATER)
			canSpawn = true;
		else {
			x = (rand() % m_Columns) * m_Size;
			y = (rand() % m_Rows) * m_Size;
		}
	}

	Ship* ship = new Ship({ x, y, SHIP_Z_INDEX });
	m_Manager->addEntity(ship);
	m_Manager->addEntity(new Player({ x, y, PLAYER_Z_INDEX }, ship->getID()));
}