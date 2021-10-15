#include "Texture.h"
#include <iostream>
#include <GL/glew.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(std::string path) : m_Path(path) { 
	glGenTextures(1, &m_ID);
	glBindTexture(GL_TEXTURE_2D, m_ID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	unsigned char* data = stbi_load(path.c_str(), &m_Width, &m_Height, &m_NumChannels, 0);

	if (data) {
		if (m_NumChannels == 3)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		else if (m_NumChannels == 4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		else {
			std::cout << "[TEXTURE] No support for " << m_NumChannels << " channel(s)!" << std::endl;
			m_Success = false;
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cerr << "[TEXTURE] " << path << " not found!" << std::endl;
		m_Success = false;
	}

	stbi_image_free(data);
}

Texture::~Texture() {
	glDeleteTextures(1, &m_ID);
	std::cout << "[TEXTURE] " << m_Path << " deleted!" << std::endl;
}

void Texture::bind(int index) {
	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(GL_TEXTURE_2D, m_ID);
}

void Texture::unbind() {
	glBindTexture(GL_TEXTURE_2D, 0);
}

int Texture::getWidth() const {
	return m_Width;
}

int Texture::getHeight() const {
	return m_Height;
}

int Texture::getNumChannels() const {
	return m_NumChannels;
}

bool Texture::getSuccess() const {
	return m_Success;
}

std::string Texture::getPath() const {
	return m_Path;
}