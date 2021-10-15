#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "Sprite2D.h"
#include "Shader.h"

class Game;
class Manager;

enum class EntityType {
	PLAYER,
	TILE,
	ENEMY,
	SHIP,
	GARBAGE
};

class Entity {
protected:
	static Game* m_GamePtr;
	static Manager* m_ManagerPtr;
	static int m_IDCount;

	float m_Size;
	float m_Margin = 0.0f;
	float m_MovementSpeed = 1.0f;
	int m_IslandID = -1;
	glm::vec3 m_Position;
	glm::mat4 m_Model;

	bool m_FoundPath;
	std::vector<glm::vec3> m_Path;

	bool m_Active = true;
	int m_ID;

	EntityType m_EntityType;

	Shader* m_Shader;
	Sprite2D* m_Sprite;
protected:
	void moveRandomly();
	void moveTowardsPlayer();
	bool pathFind();
	void pathFindRecursive(glm::vec3 pathPos);

	virtual void updateModelMatrix();

	bool collide();
	virtual bool tileCollision();
	virtual bool entityCollision();
public:
	static void setGamePointer(Game* game);

	Entity(glm::vec3 position);
	Entity(glm::vec3 position, int islandID);
	~Entity();

	bool isActive();
	void destroy();

	// TODO support for different sizes from both entities
	bool intersects(Entity* e);
	//bool intersects(glm::vec2 pos, glm::vec2 size = glm::vec2(32.0f));
	float distanceBetween(Entity* e);

	virtual void update();
	virtual void render();

	int getID() const;
	int getIslandID() const;
	const glm::vec3& getPosition() const;
	float getSize() const;
	float getMargin() const;
	float getMovementSpeed() const;
	EntityType getEntityType();
};

