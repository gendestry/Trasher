#include "Pause.h"


Pause::Pause(GLFWwindow* window, int width, int height) : GUI(window, width, height) {
	m_Sprite = new Sprite2D("res/pausemenu.png");
}

Pause::~Pause() {

}
