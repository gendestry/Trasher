#pragma once
#include <vector>
#include <string>

class Texture {
	unsigned int m_ID;
	int m_Width, m_Height, m_NumChannels;
	bool m_Success = true;

	std::string m_Path;
public:
	//Texture();
	Texture(std::string path);
	~Texture();

	void bind(int index);
	void unbind();

	int getWidth() const;
	int getHeight() const;
	int getNumChannels() const;

	bool getSuccess() const;

	std::string getPath() const;
};

