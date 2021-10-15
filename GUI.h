#pragma once
#include "GL/glew.h"
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include <vector>
#include <functional>
#include "Sprite2D.h"
#include "Shader.h"


using CallbackFunction = std::function<void()>;

class GUI {
protected:
	struct Button {
		float x, y;
		float width, height;
		CallbackFunction callbackfunc;

		Button(float bx, float by, float bwidth, float bheight, CallbackFunction callback)
			: x(bx), y(by), width(bwidth), height(bheight), callbackfunc(callback) {}

		bool intersects(float ix, float iy) {
			if (ix > x && ix < x + width && iy > y && iy < y + height)
				return true;

			return false;
		}
	};

	const float BUTTON_Z_INDEX = 0.91f;
	const float BACKGROUND_Z_INDEX = 0.9f;

	GLFWwindow* m_Window;

	int m_Width, m_Height;
	glm::mat4 m_Model, m_Proj;

	double m_MouseX, m_MouseY;

	Sprite2D* m_Sprite, *m_ButtonSprite;
	Shader* m_Shader, *m_ButtonShader;

	std::vector<Button> m_Buttons;
public:
	GUI(GLFWwindow* window, int width, int height);
	~GUI();

	virtual void handleEvents();
	void addButton(float x, float y, float width, float height, CallbackFunction func);
	void render();
};

