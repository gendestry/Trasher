#include "ResourceManager.h"


ResourceManager::ResourceManager() {

}

ResourceManager::~ResourceManager() {
	for (auto& it : m_Shaders) {
		delete it.second;
	}

	for (auto& it : m_Textures) {
		delete it.second;
	}

	for (auto& it : m_Sprites) {
		delete it.second;
	}
}

void ResourceManager::addShader(const char* name, Shader* shader) {
	if (m_Shaders.find(name) == m_Shaders.end())
		m_Shaders[name] = shader;
}

void ResourceManager::addTexture(const char* name, Texture* texture) {
	if (m_Textures.find(name) == m_Textures.end())
		m_Textures[name] = texture;
}

void ResourceManager::addSprite(const char* name, Sprite2D* sprite) {
	if (m_Sprites.find(name) == m_Sprites.end())
		m_Sprites[name] = sprite;
}

Shader* ResourceManager::getShader(const char* name) {
	if (m_Shaders.find(name) != m_Shaders.end())
		return m_Shaders[name];

	return nullptr;
}

Texture* ResourceManager::getTexture(const char* name) {
	if (m_Textures.find(name) != m_Textures.end())
		return m_Textures[name];

	return nullptr;
}

Sprite2D* ResourceManager::getSprite(const char* name) {
	if (m_Sprites.find(name) != m_Sprites.end())
		return m_Sprites[name];

	return nullptr;
}