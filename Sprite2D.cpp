#include "Sprite2D.h"

unsigned int Sprite2D::m_VAO, Sprite2D::m_VBO;


Sprite2D::Sprite2D(Texture* texture) : m_Texture(texture) {}

Sprite2D::Sprite2D(const char* texPath) {
	deleteTexture = true;
	m_Texture = new Texture(texPath);
}

Sprite2D::~Sprite2D() {
	if (deleteTexture)
		delete m_Texture;
}

void Sprite2D::render() {
	glBindVertexArray(m_VAO);
	m_Texture->bind(0);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Sprite2D::CreateBuffers() {
	float vertexes[] = {
		// pos[3], normal[3], texcoords[2]
		0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,	// top left
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,	// bottom left
		1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,	// bottom right
		1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,	// bottom right
		1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,	// top right
		0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,	// top left
	};

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexes), &vertexes[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	glBindVertexArray(0);
}

void Sprite2D::Cleanup() {
	glDeleteBuffers(1, &m_VBO);
	glDeleteVertexArrays(1, &m_VAO);
}
