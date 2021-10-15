#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "ResourceManager.h"
#include "Manager.h"
#include "Level.h"
#include <GLFW/glfw3.h>
#include "Menu.h"
#include "Pause.h"

class Game {
	int m_Width;
	int m_Height;
	GLFWwindow* m_Window;
	
	ResourceManager m_ResourceManager;
	Manager m_Manager;
	Level* m_Level;

	float m_FogOfWar;

	glm::mat4 m_Projection;

	Menu* m_Menu;
	Pause* m_Pause;

	enum class GameState {
		MENU,
		GAME,
		PAUSE,
		SCOREBOARD,
		SETTINGS
	} m_GameState = GameState::MENU;

private:
	void intialiseResourceManager();
	void setFogOfWar(float numTiles);
public:
	Game(int width, int height, GLFWwindow* window);
	~Game();

	void handleEvents();
	void update();
	void render();

	ResourceManager& getResourceManager();
	Manager& getManager();
	Level* getLevel();

	int getWidth() const;
	int getHeight() const;

	const glm::mat4& getProjectionMatrix() const;
	float getEntitySize();
};

