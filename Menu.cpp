#include "Menu.h"


Menu::Menu(GLFWwindow* window, int width, int height) : GUI(window, width, height) {
	m_Sprite = new Sprite2D("res/mainmenu.png");
}


Menu::~Menu() {
}
