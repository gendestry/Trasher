#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Game.h"

const int WIDTH = 800;
const int HEIGHT = 608;

GLFWwindow* window;
Game* game;

void init() {
	if (!glfwInit())
		std::cerr << "[GLFW] Could not initialize!" << std::endl;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	window = glfwCreateWindow(WIDTH, HEIGHT, "Trasher v1.0", NULL, NULL);

	if (!window) {
		std::cerr << "[GLFW] Window not created!" << std::endl;
		glfwTerminate();
	}

	glfwMakeContextCurrent(window);

	if (glewInit() != 0)
		std::cerr << "[GLEW] Could not initialise!" << std::endl;

	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// class initialisation
	game = new Game(WIDTH, HEIGHT, window);
}

int main() {
	init();

	while (!glfwWindowShouldClose(window)) {
		game->handleEvents();	// key and mouse input
		game->update();			// updates
		game->render();			// rendering
	}

	delete game;

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
