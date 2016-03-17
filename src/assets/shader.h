#pragma once

#include <glm/glm.hpp>
#include <gl/glew.h>

class Shader
{
private:
	GLuint fragmentShader;
	GLuint vertexShader;
	GLuint program;

	GLint getUniLoc(GLuint, const char*);
	void printShaderLogInfo(GLuint);
	char *textFileRead(const char*);

public:
	static void unbind();

	Shader(const char *vertexCode, const char *fragmentCode);
	~Shader();

	void bind();
	void link();

	void bindAttrib(const char*, unsigned int);
	void uniform1i(const char*, int);
	void uniform1f(const char*, float);
	void uniform1fv(const char*, int, float*);
	void uniform2f(const char*, float, float);
	void uniform2fv(const char*, int, float*);
	void uniformVec2(const char*, glm::vec2);
	void uniform3iv(const char*, int, int*);
	void uniform3fv(const char*, int, float*);
	void uniform3f(const char*, const float, const float, const float);
	void uniformVec3(const char*, glm::vec3);
	void uniformMatrix3fv(const char*, int, GLfloat*, bool = false);
	void uniform4iv(const char*, int, int*);
	void uniform4fv(const char*, int, float*);
	void uniform4f(const char*, float, float, float, float);
	void uniformVec4(const char*, glm::vec4);
	void uniformMatrix4fv(const char*, int, GLfloat*, bool = false);
};
