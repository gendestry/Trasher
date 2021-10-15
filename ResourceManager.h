#pragma once
#include "Shader.h"
#include "Texture.h"
#include "Sprite2D.h"
#include <unordered_map>

class ResourceManager {
	std::unordered_map<const char*, Shader*> m_Shaders;
	std::unordered_map<const char*, Texture*> m_Textures;
	std::unordered_map<const char*, Sprite2D*> m_Sprites;
public:
	ResourceManager();
	~ResourceManager();

	void addShader(const char* name, Shader* shader);
	void addTexture(const char* name, Texture* texture);
	void addSprite(const char* name, Sprite2D* sprite);

	Shader* getShader(const char* name);
	Texture* getTexture(const char* name);
	Sprite2D* getSprite(const char* name);
};

