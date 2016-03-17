#pragma once

#include <gl/glew.h>

#include <map>
#include <string>

class Shader;

class AssetManager
{
private:
	static std::map<std::string, Shader*> shaders;
	static std::map<std::string, GLuint> textures;

public:
	static Shader *getShader(std::string name);
	static GLuint getTexture(std::string name, bool mipmapping = true, bool wrap = true, bool flipVertical = true);
};
