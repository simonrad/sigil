#pragma once

#include "config.h"

#ifdef USE_GLES
	#include <GLES2/gl2.h>
	#include <GLES2/gl2ext.h>
#else
	#include <GL/glew.h>
#endif

typedef struct SHADER
{
	GLuint program;
	GLuint vertexShader;
	GLuint fragmentShader;
} Shader;

Shader *shaderLoad(const char *vertexCode, const char *fragmentCode);
void shaderLink(Shader *shader);
void shaderBind(Shader *shader);
void shaderUnbind();
void shaderBindAttrib(Shader *shader, const char*, unsigned int);
void shaderFinalize(Shader *shader);

void shaderUniform1i(Shader *shader, const char*, int);
void shaderUniform1f(Shader *shader, const char*, float);
void shaderUniform1fv(Shader *shader, const char*, int, float*);
void shaderUniform2f(Shader *shader, const char*, float, float);
void shaderUniform2fv(Shader *shader, const char*, int, float*);
void shaderUniform3iv(Shader *shader, const char*, int, int*);
void shaderUniform3fv(Shader *shader, const char*, int, float*);
void shaderUniform3f(Shader *shader, const char*, const float, const float, const float);
void shaderUniformMatrix3fv(Shader *shader, const char*, int, GLfloat*);
void shaderUniform4iv(Shader *shader, const char*, int, int*);
void shaderUniform4fv(Shader *shader, const char*, int, float*);
void shaderUniform4f(Shader *shader, const char*, float, float, float, float);
void shaderUniformMatrix4fv(Shader *shader, const char*, int, GLfloat*);
