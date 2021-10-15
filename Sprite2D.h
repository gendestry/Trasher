#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Texture.h"

class Sprite2D {
	static unsigned int m_VAO;
	static unsigned int m_VBO;

	Texture* m_Texture;

	bool deleteTexture = false;
public:
	static void CreateBuffers();
	static void Cleanup();

	Sprite2D(Texture* texture);
	Sprite2D(const char* texPath);
	~Sprite2D();

	void render();
};