#include "GUI.h"
#include "glm/gtc/matrix_transform.hpp"

GUI::GUI(GLFWwindow* window, int width, int height) : m_Window(window), m_Width(width), m_Height(height) {
	m_Model = m_Proj = glm::mat4();
	m_Model = glm::translate(m_Model, { 0.0f, 0.0f, BACKGROUND_Z_INDEX });
	m_Model = glm::scale(m_Model, { width, height, 1.0f });
	m_Proj = glm::ortho(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);

	m_ButtonSprite = new Sprite2D("res/button.png");

	m_Shader = new Shader("shaders/gui.vert","shaders/gui.frag");
	m_Shader->use();
	m_Shader->setMat4("uModel", m_Model);
	m_Shader->setMat4("uProjection", m_Proj);
	m_Shader->disable();

	m_ButtonShader = new Shader("shaders/gui_button.vert", "shaders/gui_button.frag");
	m_ButtonShader->use();
	m_ButtonShader->setMat4("uProjection", m_Proj);
	m_ButtonShader->setBool("uHovering", false);
	m_ButtonShader->disable();
}


GUI::~GUI() {
	delete m_ButtonSprite;
	delete m_Sprite;
	delete m_Shader;
	delete m_ButtonShader;
}

void GUI::handleEvents() {
	static bool canPress = true;
	glfwGetCursorPos(m_Window, &m_MouseX, &m_MouseY);

	if (glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && canPress) {
		for (auto& b : m_Buttons)
			if (b.intersects(m_MouseX, m_MouseY)) {
				b.callbackfunc();
				canPress = false;
			}
	}

	if (glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE && !canPress)
		canPress = true;
}

void GUI::addButton(float x, float y, float width, float height, CallbackFunction func) {
	m_Buttons.emplace_back(x, y, width, height, func);
}

void GUI::render() {
	// render background
	m_Shader->use();
	m_Sprite->render();
	m_Shader->disable();

	static glm::mat4 buttonModel;
	m_ButtonShader->use();
	for (auto& b : m_Buttons) {
		buttonModel = glm::mat4();
		buttonModel = glm::translate(buttonModel, { b.x, b.y, BUTTON_Z_INDEX });
		buttonModel = glm::scale(buttonModel, { b.width, b.height, 1.0f });
		if (b.intersects(m_MouseX, m_MouseY)) {
			m_ButtonShader->setBool("uHovering", true);
		}
		else {
			m_ButtonShader->setBool("uHovering", false);
		}
		m_ButtonShader->setMat4("uModel", buttonModel);
		m_ButtonSprite->render();
	}
	m_ButtonShader->disable();
}
