#include "Game.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

Game::Game(int width, int height, GLFWwindow* window) : m_Width(width), m_Height(height), m_Window(window) {
	Sprite2D::CreateBuffers();
	Entity::setGamePointer(this);

	m_Projection = glm::ortho(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);
	
	intialiseResourceManager();

	// MENU BUTTONS SETUP
	m_Menu = new Menu(m_Window, width, height);
	m_Menu->addButton(250.0f, 220.0f, 300.0f, 50.0f, [this]() {		// new game
		m_GameState = GameState::GAME;
		m_Level->loadFromFile("levels/level_02.lvl");
		setFogOfWar(12);
	});
	m_Menu->addButton(250.0f, 290.0f, 300.0f, 50.0f, [this]() {		// load game
		m_GameState = GameState::GAME;
		m_Level->loadFromSave();
		setFogOfWar(12);
	});
	m_Menu->addButton(250.0f, 360.0f, 300.0f, 50.0f, [this]() {		// show scoreboard
		//m_GameState = GameState::SCOREBOARD;
		std::cout << "Scoreboard!" << std::endl;
		m_Level->loadScoreboard();
	});
	m_Menu->addButton(250.0f, 430.0f, 300.0f, 50.0f, [this]() {		// quit
		glfwSetWindowShouldClose(m_Window, true);
	});
	m_Menu->addButton(740.0f, 550.0f, 40.0f, 40.0f, [this]() {		// settings
		//m_GameState = GameState::SETTINGS;
		std::cout << "Settings!" << std::endl;
	});
	
	// PAUSE BUTTONS SETUP
	m_Pause = new Pause(m_Window, width, height);
	m_Pause->addButton(250.0f, 220.0f, 300.0f, 50.0f, [this]() {	// resume
		m_GameState = GameState::GAME;
	});

	m_Pause->addButton(250.0f, 290.0f, 300.0f, 50.0f, [this]() {	// save game
		m_Level->saveGame();
	});

	m_Pause->addButton(250.0f, 360.0f, 300.0f, 50.0f, [this]() {	// quit to main menu
		m_GameState = GameState::MENU;
	});

	m_Level = new Level(&m_Manager);
	// add to level
	//setFogOfWar(12);

}

Game::~Game() {
	delete m_Level;
	Sprite2D::Cleanup();
}

void Game::intialiseResourceManager() {
	// Sprites
	m_ResourceManager.addSprite("player", new Sprite2D("res/awesomeface.png"));
	m_ResourceManager.addSprite("enemy", new Sprite2D("res/enemy.png"));
	m_ResourceManager.addSprite("ship", new Sprite2D("res/ship.png"));
	m_ResourceManager.addSprite("garbage", new Sprite2D("res/poop_emoji.png"));

	m_ResourceManager.addSprite("tile_grass", new Sprite2D("res/grass.png"));
	m_ResourceManager.addSprite("tile_sand", new Sprite2D("res/sand.png"));
	m_ResourceManager.addSprite("tile_water", new Sprite2D("res/water.png"));
	m_ResourceManager.addSprite("tile_mountain", new Sprite2D("res/mountain.png"));

	
	// Shaders
	m_ResourceManager.addShader("tile", new Shader("shaders/tile.vert", "shaders/tile.frag"));
	m_ResourceManager.addShader("entity", new Shader("shaders/entity.vert", "shaders/entity.frag"));
	m_ResourceManager.addShader("player", new Shader("shaders/player.vert", "shaders/player.frag"));
	
	Shader* temp = m_ResourceManager.getShader("tile");
	temp->use();
	temp->setInt("uTextureIndex", 0);
	temp->setFloat("uFogOfWar", m_FogOfWar);
	temp->setMat4("uProjection", m_Projection);
	temp->disable();

	temp = m_ResourceManager.getShader("entity");
	temp->use();
	temp->setInt("uTextureIndex", 0);
	temp->setFloat("uFogOfWar", m_FogOfWar);
	temp->setMat4("uProjection", m_Projection);
	temp->disable();

	temp = m_ResourceManager.getShader("player");
	temp->use();
	temp->setInt("uTextureIndex", 0);
	temp->setMat4("uProjection", m_Projection);
	temp->disable();
}

void Game::setFogOfWar(float numTiles) {
	m_FogOfWar = numTiles * getEntitySize();

	Shader* temp = m_ResourceManager.getShader("tile");
	temp->use();
	temp->setFloat("uFogOfWar", m_FogOfWar);
	temp->disable();

	temp = m_ResourceManager.getShader("entity");
	temp->use();
	temp->setFloat("uFogOfWar", m_FogOfWar);
	temp->disable();
}

void Game::handleEvents() {
	if (m_GameState == GameState::MENU) {
		m_Menu->handleEvents();

		if (glfwGetKey(m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(m_Window, true);
	}
	else if (m_GameState == GameState::GAME) {
		// left & right
		if (glfwGetKey(m_Window, GLFW_KEY_A) == GLFW_PRESS)
			m_Manager.getPlayer()->move({ -1.0f, 0.0f });
		else if (glfwGetKey(m_Window, GLFW_KEY_D) == GLFW_PRESS)
			m_Manager.getPlayer()->move({ 1.0f, 0.0f });

		// up & down
		if (glfwGetKey(m_Window, GLFW_KEY_W) == GLFW_PRESS)
			m_Manager.getPlayer()->move({ 0.0f, -1.0f });
		else if (glfwGetKey(m_Window, GLFW_KEY_S) == GLFW_PRESS)
			m_Manager.getPlayer()->move({ 0.0f, 1.0f });

		// toggle ship
		if (glfwGetKey(m_Window, GLFW_KEY_F) == GLFW_PRESS)
			m_Manager.getPlayer()->toggleShip();

		// pause
		if (glfwGetKey(m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			m_GameState = GameState::PAUSE;
	}
	else if (m_GameState == GameState::PAUSE) {
		m_Pause->handleEvents();
		/*if (glfwGetKey(m_Window, GLFW_KEY_1) == GLFW_PRESS)
			m_GameState = GameState::GAME;
		else if (glfwGetKey(m_Window, GLFW_KEY_2) == GLFW_PRESS)
			m_Level->saveGame();

		if (glfwGetKey(m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			m_GameState = GameState::MENU;*/
	}
}

void Game::update() {
	static double oldTime = 0.0;
	double now = glfwGetTime();
	double deltaTime = (now - oldTime) * 10.0f;
	oldTime = now;

	if (m_GameState == GameState::GAME) {
		m_Manager.update(deltaTime);

		static Shader* temp;
		auto& ppos = m_Manager.getPlayer()->getPosition();
		float size = m_Manager.getPlayer()->getSize();

		temp = m_ResourceManager.getShader("entity");
		temp->use();
		temp->setVec3("uPlayerPos", { ppos.x + size / 2, ppos.y + size / 2, ppos.z });
		temp->setFloat("uTime", now);
		temp->disable();

		temp = m_ResourceManager.getShader("tile");
		temp->use();
		temp->setVec3("uPlayerPos", { ppos.x + size / 2, ppos.y + size / 2, ppos.z });
		temp->setFloat("uTime", now);
		temp->disable();

		if (m_Manager.isLevelComplete()) {
			m_Level->saveScoreboard();
			m_GameState = GameState::MENU;
		}
	}
}

void Game::render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	if (m_GameState == GameState::MENU)
		m_Menu->render();
	else if (m_GameState == GameState::GAME)
		m_Manager.render();
	else if (m_GameState == GameState::PAUSE) {
		m_Manager.render();
		m_Pause->render();
	}

	glfwSwapBuffers(m_Window);
	glfwPollEvents();
}

ResourceManager& Game::getResourceManager() {
	return m_ResourceManager;
}

Manager& Game::getManager() {
	return m_Manager;
}

Level* Game::getLevel() {
	return m_Level;
}

int Game::getWidth() const {
	return m_Width;
}

int Game::getHeight() const {
	return m_Height;
}

const glm::mat4& Game::getProjectionMatrix() const {
	return m_Projection;
}

float Game::getEntitySize() {
	return m_Level->getEntitySize();
}